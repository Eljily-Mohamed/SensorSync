#ifndef _MMA7660_H_
#define _MMA7660_H_

#include "include/board.h"

/* Active Mode: sample rate options [Samples per Second] */
#define	MMA7660_AMPD		0
#define	MMA7660_AM64		1
#define	MMA7660_AM32		2
#define	MMA7660_AM16		3
#define	MMA7660_AM8			4
#define	MMA7660_AM4			5
#define	MMA7660_AM2			6
#define	MMA7660_AM1			7

/* Auto Wake mode: sample rate options [Samples per Second] */
#define MMA7660_AW32		(0<<3)
#define MMA7660_AW16		(1<<3)
#define MMA7660_AW8			(2<<3)
#define MMA7660_AW1			(3<<3)

/* Number of samples used to update position (portrait/landscape) */
#define MMA7660_FILT_NONE	(0<<5)
#define MMA7660_FILT_2S		(1<<5)
#define MMA7660_FILT_3S		(2<<5)
#define MMA7660_FILT_4S		(3<<5)
#define MMA7660_FILT_5S		(4<<5)
#define MMA7660_FILT_6S		(5<<5)
#define MMA7660_FILT_7S		(6<<5)
#define MMA7660_FILT_8S		(7<<5)

/* Operating mode */
#define MMA7660_ACTIVE		(1<<8)		/* Set the device active */
#define MMA7660_AW_ON		(1<<11)		/* Auto Wake on */
#define MMA7660_AS_ON		(1<<12)		/* Auto Sleep on */

int mma7660_setup(uint32_t mode);

int mma7660_read_XYZT(int32_t *data);


#endif
