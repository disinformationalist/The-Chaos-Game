#include "chaos.h"
#include <lcms2.h>


/* static int color_distance(uint8_t *a, uint8_t *b)
{
	return abs(a[0] - b[0]) + abs(a[1] - b[1]) + abs(a[2] - b[2]);
} */

static int proof_rgb_color(int rgb_hex, cmsHTRANSFORM proof)
{
	uint8_t rgb_in[3] = {
		(rgb_hex >> 16) & 0xFF,
		(rgb_hex >> 8) & 0xFF,
		rgb_hex & 0xFF
	};

	uint8_t rgb_out[3];
	cmsDoTransform(proof, rgb_in, rgb_out, 1);

	// Repack proofed RGB to int
	return (rgb_out[0] << 16) | (rgb_out[1] << 8) | rgb_out[2];
}

void convert_colors_to_cmyk_safe(t_colors *colors)
{
	// Load ICC profiles
/* 	cmsHPROFILE srgb = cmsOpenProfileFromFile("sRGB.icc", "r");
	cmsHPROFILE fogra = cmsOpenProfileFromFile("ISOcoated_v2_eci.icc", "r"); */

	cmsHPROFILE srgb = cmsOpenProfileFromFile("/usr/share/color/icc/colord/sRGB.icc", "r");
	cmsHPROFILE fogra = cmsOpenProfileFromFile("/usr/share/color/icc/colord/FOGRA39L_coated.icc", "r");


	if (!srgb || !fogra) {
		fprintf(stderr, "ICC profile load failed\n");
		exit(1);
	}

	// Create soft-proof transform
	cmsHTRANSFORM proof = cmsCreateProofingTransform(
		srgb, TYPE_RGB_8,
		srgb, TYPE_RGB_8,
		fogra,
		INTENT_RELATIVE_COLORIMETRIC, INTENT_RELATIVE_COLORIMETRIC,
		cmsFLAGS_SOFTPROOFING | cmsFLAGS_BLACKPOINTCOMPENSATION
	);

	if (!proof) {
		fprintf(stderr, "Transform creation failed\n");
		exit(1);
	}

	// Apply proofing to each color
	colors->color_1 = proof_rgb_color(colors->color_1, proof);
	colors->color_2 = proof_rgb_color(colors->color_2, proof);
	colors->color_3 = proof_rgb_color(colors->color_3, proof);
	colors->color_4 = proof_rgb_color(colors->color_4, proof);
	//colors->background = proof_rgb_color(colors->background, proof);

	// Clean up
	cmsDeleteTransform(proof);
	cmsCloseProfile(srgb);
	cmsCloseProfile(fogra);
}

//convert final image to cmyk, make a pointer swap set to key press so that view/print swaps between either

void cmyk_softproof_image(t_game *r, t_img *src, t_img *dst)
{
	int x, y;
	uint8_t *src_px, *dst_px;
	cmsHPROFILE srgb = cmsOpenProfileFromFile("/usr/share/color/icc/colord/sRGB.icc", "r");
	cmsHPROFILE fogra = cmsOpenProfileFromFile("/usr/share/color/icc/colord/FOGRA39L_coated.icc", "r");

	if (!srgb || !fogra) {
		fprintf(stderr, "ICC profile load failed\n");
		exit(1);
	}

	cmsHTRANSFORM proof = cmsCreateProofingTransform(
		srgb, TYPE_RGB_8,
		srgb, TYPE_RGB_8,
		fogra,
		INTENT_RELATIVE_COLORIMETRIC, INTENT_RELATIVE_COLORIMETRIC,
		cmsFLAGS_SOFTPROOFING | cmsFLAGS_BLACKPOINTCOMPENSATION
	);

	if (!proof) {
		fprintf(stderr, "Transform creation failed\n");
		exit(1);
	}

	for (y = 0; y < r->height_orig; ++y)
	{
		for (x = 0; x < r->width_orig; ++x)
		{
			src_px = (uint8_t *)(src->pixels_ptr + y * src->line_len + x * (src->bpp / 8));
			dst_px = (uint8_t *)(dst->pixels_ptr + y * dst->line_len + x * (dst->bpp / 8));

			uint8_t rgb_in[3] = { src_px[2], src_px[1], src_px[0] };  // BGR to RGB
			uint8_t rgb_out[3];

			cmsDoTransform(proof, rgb_in, rgb_out, 1);

			dst_px[0] = rgb_out[2];  // B
			dst_px[1] = rgb_out[1];  // G
			dst_px[2] = rgb_out[0];  // R
			if (src->bpp == 32)
				dst_px[3] = src_px[3]; // copy alpha
		}
	}

	cmsDeleteTransform(proof);
	cmsCloseProfile(srgb);
	cmsCloseProfile(fogra);
}
