/*
 * Copyright (c) 2011-2012 - Mauro Carvalho Chehab
 * Copyright (c) 2012 - Andre Roth <neolynx@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * Or, point your browser to http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 *
 */

#include <linux/videodev2.h>

#include "descriptors/desc_sat.h"
#include "descriptors.h"
#include "dvb-fe.h"

void dvb_desc_sat_init(struct dvb_v5_fe_parms *parms, const uint8_t *buf, struct dvb_desc *desc)
{
	struct dvb_desc_sat *sat = (struct dvb_desc_sat *) desc;
	/* copy from .length */
	memcpy(((uint8_t *) sat ) + sizeof(sat->type) + sizeof(sat->next) + sizeof(sat->length),
		buf,
		sat->length);
	bswap16(sat->orbit);
	bswap32(sat->bitfield);
	bswap32(sat->frequency);
	sat->orbit = bcd(sat->orbit);
	sat->frequency   = bcd(sat->frequency) * 10;
	sat->symbol_rate = bcd(sat->symbol_rate) * 100;
}

void dvb_desc_sat_print(struct dvb_v5_fe_parms *parms, const struct dvb_desc *desc)
{
	const struct dvb_desc_sat *sat = (const struct dvb_desc_sat *) desc;
	char pol;
	switch(sat->polarization) {
		case 0:
			pol = 'H';
			break;
		case 1:
			pol = 'V';
			break;
		case 2:
			pol = 'L';
			break;
		case 3:
			pol = 'R';
			break;
	}
	dvb_log("|           modulation_system %s", sat->modulation_system ? "DVB-S2" : "DVB-S");
	dvb_log("|           frequency         %d %c", sat->frequency, pol);
	dvb_log("|           symbol_rate       %d", sat->symbol_rate);
	dvb_log("|           fec               %d", sat->fec);
	dvb_log("|           modulation_type   %d", sat->modulation_type);
	dvb_log("|           roll_off          %d", sat->roll_off);
	dvb_log("|           orbit             %.1f %c", (float) sat->orbit / 10.0, sat->west_east ? 'E' : 'W');
}

const unsigned dvbs_dvbc_dvbs_freq_inner[] = {
	[0]  = FEC_AUTO,
	[1]  = FEC_1_2,
	[2]  = FEC_2_3,
	[3]  = FEC_3_4,
	[4]  = FEC_5_6,
	[5]  = FEC_7_8,
	[6]  = FEC_8_9,
	[7]  = FEC_3_5,
	[8]  = FEC_4_5,
	[9]  = FEC_9_10,
	[10 ...14] = FEC_AUTO,	/* Currently, undefined */
	[15] = FEC_NONE,
};

const unsigned dvbs_polarization[] = {
	[0] = POLARIZATION_H,
	[1] = POLARIZATION_V,
	[2] = POLARIZATION_L,
	[3] = POLARIZATION_R
};

const unsigned dvbs_rolloff[] = {
	[0] = ROLLOFF_35,
	[1] = ROLLOFF_25,
	[2] = ROLLOFF_20,
	[3] = ROLLOFF_AUTO,	/* Reserved */
};

const unsigned dvbs_modulation[] = {
	[0] = QAM_AUTO,
	[1] = QPSK,
	[2] = PSK_8,
	[3] = QAM_16
};

