#pragma once

#include <stdint.h>


typedef struct
{
	uint64_t CalculateCameraAngle_CallSite;
	uint64_t SaveToInputConfigFile_CallSite;
	uint64_t instr_roll_movss;
	uint64_t instr_zoom_movss;
} Offsets_t;
static Offsets_t Offsets =
{
    .CalculateCameraAngle_CallSite = 0x02c1e3aa,
	.SaveToInputConfigFile_CallSite = 0x06506c69,
	.instr_roll_movss = 0x2c21306,
	.instr_zoom_movss = 0x2C1EC8D
};
