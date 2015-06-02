/*
 *  Copyright (C) 2015 Silver Bullet Technology LLC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef S3MA_IRQS_H_
#define S3MA_IRQS_H_


/*
 *
	PARITYFAIL[7] BTAC parity error
	PARITYFAIL[6] GHB parity error
	PARITYFAIL[5] instruction tag RAM parity error
	PARITYFAIL[4] instruction data RAM parity error
	PARITYFAIL[3] main TLB parity error
	PARITYFAIL[2] data outer RAM parity error
	PARITYFAIL[1] data tag RAM parity error
	PARITYFAIL[0] data data RAM parity error.

	interrupts, maximum of 224
	assign irqs [  7:  0] = PARITYFAIL0_sync;      //  8
	assign irqs [ 15:  8] = PARITYFAIL1_sync;      //  8
	assign irqs [ 23: 16] = PARITYFAIL2_sync;      //  8
	assign irqs [ 26: 24] = {(|PARITYFAIL3_sync[7:2]), PARITYFAIL3_sync[1:0]}; // 3
	assign irqs [ 27    ] = SCUEVABORT_sync;       //  1
	assign irqs [ 28    ] = l2ccintr;              //  1
	assign irqs [ 29    ] = ocm_rx_interrupt;      //  1
	assign irqs [ 30    ] = ocm_tx_interrupt;      //  1
	assign irqs [ 31    ] = dma330_abort_interrupt;//  1
	assign irqs [ 35: 32] = PARITYFAILSCU_sync;    //  4
	assign irqs [ 59: 36] = gpio_interrupt;        // 24
	assign irqs [ 64: 60] = spi_interrupt;         //  5
	assign irqs [ 67: 65] = timer_interrupt;       //  3
	assign irqs [ 69: 68] = uart_interrupt;        //  2
	assign irqs [ 70    ] = i2c_interrupt;         //  1
	assign irqs [ 71    ] = usb0_interrupt;        //  1
	assign irqs [ 72    ] = usb1_interrupt;        //  1
	assign irqs [ 73    ] = sd_interrupt;          //  1
	assign irqs [113: 74] = dw_dma_interrupt;      // 40
	assign irqs [145:114] = dma330_interrupt;      // 32
	assign irqs [146    ] = wfe_interrupt;         //  1
	assign irqs [147    ] = dw_ahb_arb_interrupt;  //  1
	assign irqs [155:148] = diceu_interrupt;       //  8
	assign irqs [175:156] = ~llm_interrupt_n;      // 20
	assign irqs [179:176] = ~CTIRQn;               //  4
	assign irqs [201:180] = {usb0_endp_interrupt[26:16], usb0_endp_interrupt[10:0]}; // 22
	assign irqs [223:202] = {usb1_endp_interrupt[26:16], usb1_endp_interrupt[10:0]}; // 22
 *
 */

#define S3MA_IRQ_BASE	0

/* IRQ 0 */
#define S3MA_CORE0_DCACHE_DATA_PARITY_IRQ   	(S3MA_IRQ_BASE + 0)
#define S3MA_CORE0_DCACHE_TAG_PARITY_IRQ   		(S3MA_IRQ_BASE + 1)
#define S3MA_CORE0_DCACHE_OUTER_PARITY_IRQ   	(S3MA_IRQ_BASE + 2)
#define S3MA_CORE0_TLB_PARITY_IRQ   			(S3MA_IRQ_BASE + 3)
#define S3MA_CORE0_ICACHE_DATA_PARITY_IRQ 	  	(S3MA_IRQ_BASE + 4)
#define S3MA_CORE0_ICACHE_TAG_PARITY_IRQ   		(S3MA_IRQ_BASE + 5)
#define S3MA_CORE0_GHB_PARITY_IRQ   			(S3MA_IRQ_BASE + 6)
#define S3MA_CORE0_BTAC_PARITY_IRQ   			(S3MA_IRQ_BASE + 7)
#define S3MA_CORE1_DCACHE_DATA_PARITY_IRQ   	(S3MA_IRQ_BASE + 8)
#define S3MA_CORE1_DCACHE_TAG_PARITY_IRQ   		(S3MA_IRQ_BASE + 9)
/* IRQ 10 */
#define S3MA_CORE1_DCACHE_OUTER_PARITY_IRQ   	(S3MA_IRQ_BASE + 10)
#define S3MA_CORE1_TLB_PARITY_IRQ   			(S3MA_IRQ_BASE + 11)
#define S3MA_CORE1_ICACHE_DATA_PARITY_IRQ 	  	(S3MA_IRQ_BASE + 12)
#define S3MA_CORE1_ICACHE_TAG_PARITY_IRQ   		(S3MA_IRQ_BASE + 13)
#define S3MA_CORE1_GHB_PARITY_IRQ   			(S3MA_IRQ_BASE + 14)
#define S3MA_CORE1_BTAC_PARITY_IRQ   			(S3MA_IRQ_BASE + 15)
#define S3MA_CORE2_DCACHE_DATA_PARITY_IRQ   	(S3MA_IRQ_BASE + 16)
#define S3MA_CORE2_DCACHE_TAG_PARITY_IRQ   		(S3MA_IRQ_BASE + 17)
#define S3MA_CORE2_DCACHE_OUTER_PARITY_IRQ   	(S3MA_IRQ_BASE + 18)
#define S3MA_CORE2_TLB_PARITY_IRQ   			(S3MA_IRQ_BASE + 19)
/* IRQ 20 */
#define S3MA_CORE2_ICACHE_DATA_PARITY_IRQ 	  	(S3MA_IRQ_BASE + 20)
#define S3MA_CORE2_ICACHE_TAG_PARITY_IRQ   		(S3MA_IRQ_BASE + 21)
#define S3MA_CORE2_GHB_PARITY_IRQ   			(S3MA_IRQ_BASE + 22)
#define S3MA_CORE2_BTAC_PARITY_IRQ   			(S3MA_IRQ_BASE + 23)
#define S3MA_CORE3_PARITY0_IRQ   				(S3MA_IRQ_BASE + 24)
#define S3MA_CORE3_PARITY1_IRQ   				(S3MA_IRQ_BASE + 25)
#define S3MA_CORE3_PARITY2_IRQ   				(S3MA_IRQ_BASE + 26)
#define S3MA_SCUEV_ABORT_IRQ	   				(S3MA_IRQ_BASE + 27)
#define S3MA_L2_CACHE_IRQ   					(S3MA_IRQ_BASE + 28)
#define S3MA_OCM_RX_IRQ   						(S3MA_IRQ_BASE + 29)
/* IRQ 30 */
#define S3MA_OCM_TX_IRQ   						(S3MA_IRQ_BASE + 30)
#define S3MA_DMA330_ABORT_IRQ   				(S3MA_IRQ_BASE + 31)
#define S3MA_SCU_PARITY0_IRQ   					(S3MA_IRQ_BASE + 32)
#define S3MA_SCU_PARITY1_IRQ   					(S3MA_IRQ_BASE + 33)
#define S3MA_SCU_PARITY2_IRQ   					(S3MA_IRQ_BASE + 34)
#define S3MA_SCU_PARITY3_IRQ   					(S3MA_IRQ_BASE + 35)
#define S3MA_GPIO0_IRQ   						(S3MA_IRQ_BASE + 36)
#define S3MA_GPIO1_IRQ   						(S3MA_IRQ_BASE + 37)
#define S3MA_GPIO2_IRQ   						(S3MA_IRQ_BASE + 38)
#define S3MA_GPIO3_IRQ   						(S3MA_IRQ_BASE + 39)
/* IRQ 40 */
#define S3MA_GPIO4_IRQ   						(S3MA_IRQ_BASE + 40)
#define S3MA_GPIO5_IRQ   						(S3MA_IRQ_BASE + 41)
#define S3MA_GPIO6_IRQ   						(S3MA_IRQ_BASE + 42)
#define S3MA_GPIO7_IRQ   						(S3MA_IRQ_BASE + 43)
#define S3MA_GPIO8_IRQ   						(S3MA_IRQ_BASE + 44)
#define S3MA_GPIO9_IRQ   						(S3MA_IRQ_BASE + 45)
#define S3MA_GPIO10_IRQ   						(S3MA_IRQ_BASE + 46)
#define S3MA_GPIO11_IRQ   						(S3MA_IRQ_BASE + 47)
#define S3MA_GPIO12_IRQ   						(S3MA_IRQ_BASE + 48)
#define S3MA_GPIO13_IRQ   						(S3MA_IRQ_BASE + 49)
/* IRQ 50 */
#define S3MA_GPIO14_IRQ   						(S3MA_IRQ_BASE + 50)
#define S3MA_GPIO15_IRQ   						(S3MA_IRQ_BASE + 51)
#define S3MA_GPIO16_IRQ   						(S3MA_IRQ_BASE + 52)
#define S3MA_GPIO17_IRQ   						(S3MA_IRQ_BASE + 53)
#define S3MA_GPIO18_IRQ   						(S3MA_IRQ_BASE + 54)
#define S3MA_GPIO19_IRQ   						(S3MA_IRQ_BASE + 55)
#define S3MA_GPIO20_IRQ   						(S3MA_IRQ_BASE + 56)
#define S3MA_GPIO21_IRQ   						(S3MA_IRQ_BASE + 57)
#define S3MA_GPIO22_IRQ   						(S3MA_IRQ_BASE + 58)
#define S3MA_GPIO23_IRQ   						(S3MA_IRQ_BASE + 59)
/* IRQ 60 */
#define S3MA_SPI0_IRQ   						(S3MA_IRQ_BASE + 60)
#define S3MA_SPI1_IRQ   						(S3MA_IRQ_BASE + 61)
#define S3MA_SPI2_IRQ   						(S3MA_IRQ_BASE + 62)
#define S3MA_SPI3_IRQ   						(S3MA_IRQ_BASE + 63)
#define S3MA_SPI4_IRQ   						(S3MA_IRQ_BASE + 64)
#define S3MA_TIMER0_IRQ   						(S3MA_IRQ_BASE + 65)
#define S3MA_TIMER1_IRQ   						(S3MA_IRQ_BASE + 66)
#define S3MA_TIMER2_IRQ   						(S3MA_IRQ_BASE + 67)
#define S3MA_UART0_IRQ   						(S3MA_IRQ_BASE + 68)
#define S3MA_UART0_IRQ   						(S3MA_IRQ_BASE + 69)
/* IRQ 70 */
#define S3MA_I2C_IRQ   							(S3MA_IRQ_BASE + 70)
#define S3MA_USB0_IRQ   						(S3MA_IRQ_BASE + 71)
#define S3MA_USB1_IRQ   						(S3MA_IRQ_BASE + 72)
#define S3MA_MMC0_IRQ   						(S3MA_IRQ_BASE + 73)
#define S3MA_DW_DMA0_IRQ   						(S3MA_IRQ_BASE + 74)
#define S3MA_DW_DMA1_IRQ   						(S3MA_IRQ_BASE + 75)
#define S3MA_DW_DMA2_IRQ   						(S3MA_IRQ_BASE + 76)
#define S3MA_DW_DMA3_IRQ   						(S3MA_IRQ_BASE + 77)
#define S3MA_DW_DMA4_IRQ   						(S3MA_IRQ_BASE + 78)
#define S3MA_DW_DMA5_IRQ   						(S3MA_IRQ_BASE + 79)
/* IRQ 80 */
#define S3MA_DW_DMA6_IRQ   						(S3MA_IRQ_BASE + 80)
#define S3MA_DW_DMA7_IRQ   						(S3MA_IRQ_BASE + 81)
#define S3MA_DW_DMA8_IRQ   						(S3MA_IRQ_BASE + 82)
#define S3MA_DW_DMA9_IRQ   						(S3MA_IRQ_BASE + 83)
#define S3MA_DW_DMA10_IRQ   					(S3MA_IRQ_BASE + 84)
#define S3MA_DW_DMA11_IRQ   					(S3MA_IRQ_BASE + 85)
#define S3MA_DW_DMA12_IRQ   					(S3MA_IRQ_BASE + 86)
#define S3MA_DW_DMA13_IRQ   					(S3MA_IRQ_BASE + 87)
#define S3MA_DW_DMA14_IRQ   					(S3MA_IRQ_BASE + 88)
#define S3MA_DW_DMA15_IRQ   					(S3MA_IRQ_BASE + 89)
/* IRQ 90 */
#define S3MA_DW_DMA16_IRQ   					(S3MA_IRQ_BASE + 90)
#define S3MA_DW_DMA17_IRQ   					(S3MA_IRQ_BASE + 91)
#define S3MA_DW_DMA18_IRQ   					(S3MA_IRQ_BASE + 92)
#define S3MA_DW_DMA19_IRQ   					(S3MA_IRQ_BASE + 93)
#define S3MA_DW_DMA20_IRQ   					(S3MA_IRQ_BASE + 94)
#define S3MA_DW_DMA21_IRQ   					(S3MA_IRQ_BASE + 95)
#define S3MA_DW_DMA22_IRQ   					(S3MA_IRQ_BASE + 96)
#define S3MA_DW_DMA23_IRQ   					(S3MA_IRQ_BASE + 97)
#define S3MA_DW_DMA24_IRQ   					(S3MA_IRQ_BASE + 98)
#define S3MA_DW_DMA25_IRQ   					(S3MA_IRQ_BASE + 99)
/* IRQ 100 */
#define S3MA_DW_DMA26_IRQ   					(S3MA_IRQ_BASE + 100)
#define S3MA_DW_DMA27_IRQ   					(S3MA_IRQ_BASE + 101)
#define S3MA_DW_DMA28_IRQ   					(S3MA_IRQ_BASE + 102)
#define S3MA_DW_DMA29_IRQ   					(S3MA_IRQ_BASE + 103)
#define S3MA_DW_DMA30_IRQ   					(S3MA_IRQ_BASE + 104)
#define S3MA_DW_DMA31_IRQ   					(S3MA_IRQ_BASE + 105)
#define S3MA_DW_DMA32_IRQ   					(S3MA_IRQ_BASE + 106)
#define S3MA_DW_DMA33_IRQ   					(S3MA_IRQ_BASE + 107)
#define S3MA_DW_DMA34_IRQ   					(S3MA_IRQ_BASE + 108)
#define S3MA_DW_DMA35_IRQ   					(S3MA_IRQ_BASE + 109)
/* IRQ 110 */
#define S3MA_DW_DMA36_IRQ   					(S3MA_IRQ_BASE + 110)
#define S3MA_DW_DMA37_IRQ   					(S3MA_IRQ_BASE + 111)
#define S3MA_DW_DMA38_IRQ   					(S3MA_IRQ_BASE + 112)
#define S3MA_DW_DMA39_IRQ   					(S3MA_IRQ_BASE + 113)
#define S3MA_DMA330_VDEC_IRQ   					(S3MA_IRQ_BASE + 114)
#define S3MA_DMA330_LL_REQ6_IRQ   				(S3MA_IRQ_BASE + 115)
#define S3MA_DMA330_LL_REQ7_IRQ   				(S3MA_IRQ_BASE + 116)
#define S3MA_DMA330_VENC_IRQ   					(S3MA_IRQ_BASE + 117)
#define S3MA_DMA330_LL_REQ4_IRQ   				(S3MA_IRQ_BASE + 118)
#define S3MA_DMA330_LL_REQ5_IRQ   				(S3MA_IRQ_BASE + 119)
/* IRQ 120 */
#define S3MA_DMA330_UNUSED0_IRQ   				(S3MA_IRQ_BASE + 120)
#define S3MA_DMA330_LDPC_DEC0_IRQ   			(S3MA_IRQ_BASE + 121)
#define S3MA_DMA330_LDPC_ENC0_IRQ   			(S3MA_IRQ_BASE + 122)
#define S3MA_DMA330_UNUSED1_IRQ   				(S3MA_IRQ_BASE + 123)
#define S3MA_DMA330_LDPC_DEC1_IRQ   			(S3MA_IRQ_BASE + 124)
#define S3MA_DMA330_LDPC_ENC1_IRQ   			(S3MA_IRQ_BASE + 125)
#define S3MA_DMA330_LL_REQ0_IRQ   				(S3MA_IRQ_BASE + 126)
#define S3MA_DMA330_LL_REQ1_IRQ   				(S3MA_IRQ_BASE + 127)
#define S3MA_DMA330_LL_REQ2_IRQ   				(S3MA_IRQ_BASE + 128)
#define S3MA_DMA330_LL_REQ3_IRQ   				(S3MA_IRQ_BASE + 129)
/* IRQ 130 */
#define S3MA_DMA330_UNUSED2_IRQ   				(S3MA_IRQ_BASE + 130)
#define S3MA_DMA330_UNUSED3_IRQ   				(S3MA_IRQ_BASE + 131)
#define S3MA_DMA330_UNUSED4_IRQ   				(S3MA_IRQ_BASE + 132)
#define S3MA_DMA330_UNUSED5_IRQ   				(S3MA_IRQ_BASE + 133)
#define S3MA_DMA330_UNUSED6_IRQ   				(S3MA_IRQ_BASE + 134)
#define S3MA_DMA330_UNUSED7_IRQ   				(S3MA_IRQ_BASE + 135)
#define S3MA_DMA330_UNUSED8_IRQ   				(S3MA_IRQ_BASE + 136)
#define S3MA_DMA330_UNUSED9_IRQ   				(S3MA_IRQ_BASE + 137)
#define S3MA_DMA330_UNUSED10_IRQ   				(S3MA_IRQ_BASE + 138)
#define S3MA_DMA330_UNUSED11_IRQ   				(S3MA_IRQ_BASE + 139)
/* IRQ 140 */
#define S3MA_DMA330_UNUSED12_IRQ   				(S3MA_IRQ_BASE + 140)
#define S3MA_DMA330_UNUSED13_IRQ   				(S3MA_IRQ_BASE + 141)
#define S3MA_DMA330_UNUSED14_IRQ   				(S3MA_IRQ_BASE + 142)
#define S3MA_DMA330_UNUSED15_IRQ   				(S3MA_IRQ_BASE + 143)
#define S3MA_DMA330_UNUSED16_IRQ   				(S3MA_IRQ_BASE + 144)
#define S3MA_DMA330_UNUSED17_IRQ   				(S3MA_IRQ_BASE + 145)
#define S3MA_WFE_IRQ   							(S3MA_IRQ_BASE + 146)
#define S3MA_AHB_ARB_IRQ   						(S3MA_IRQ_BASE + 147)
#define S3MA_DICEU0_IRQ   						(S3MA_IRQ_BASE + 148)
#define S3MA_DICEU1_IRQ   						(S3MA_IRQ_BASE + 149)
/* IRQ 150 */
#define S3MA_DICEU2_IRQ   						(S3MA_IRQ_BASE + 150)
#define S3MA_DICEU3_IRQ   						(S3MA_IRQ_BASE + 151)
#define S3MA_DICEU4_IRQ   						(S3MA_IRQ_BASE + 152)
#define S3MA_DICEU5_IRQ   						(S3MA_IRQ_BASE + 153)
#define S3MA_DICEU6_IRQ   						(S3MA_IRQ_BASE + 154)
#define S3MA_DICEU7_IRQ   						(S3MA_IRQ_BASE + 155)
#define S3MA_LLM0_IRQ   						(S3MA_IRQ_BASE + 156)
#define S3MA_LLM1_IRQ   						(S3MA_IRQ_BASE + 157)
#define S3MA_LLM2_IRQ   						(S3MA_IRQ_BASE + 158)
#define S3MA_LLM3_IRQ   						(S3MA_IRQ_BASE + 159)
/* IRQ 160 */
#define S3MA_LLM4_IRQ   						(S3MA_IRQ_BASE + 160)
#define S3MA_LLM5_IRQ   						(S3MA_IRQ_BASE + 161)
#define S3MA_LLM6_IRQ   						(S3MA_IRQ_BASE + 162)
#define S3MA_LLM7_IRQ   						(S3MA_IRQ_BASE + 163)
#define S3MA_LLM8_IRQ   						(S3MA_IRQ_BASE + 164)
#define S3MA_LLM9_IRQ   						(S3MA_IRQ_BASE + 165)
#define S3MA_LLM10_IRQ   						(S3MA_IRQ_BASE + 166)
#define S3MA_LLM11_IRQ   						(S3MA_IRQ_BASE + 167)
#define S3MA_LLM12_IRQ   						(S3MA_IRQ_BASE + 168)
#define S3MA_LLM13_IRQ   						(S3MA_IRQ_BASE + 169)
/* IRQ 170 */
#define S3MA_LLM14_IRQ   						(S3MA_IRQ_BASE + 170)
#define S3MA_LLM15_IRQ   						(S3MA_IRQ_BASE + 171)
#define S3MA_LLM16_IRQ   						(S3MA_IRQ_BASE + 172)
#define S3MA_LLM17_IRQ   						(S3MA_IRQ_BASE + 173)
#define S3MA_LLM18_IRQ   						(S3MA_IRQ_BASE + 174)
#define S3MA_LLM19_IRQ   						(S3MA_IRQ_BASE + 175)
#define S3MA_CORESIGHT0_IRQ   					(S3MA_IRQ_BASE + 176)
#define S3MA_CORESIGHT1_IRQ   					(S3MA_IRQ_BASE + 177)
#define S3MA_CORESIGHT2_IRQ   					(S3MA_IRQ_BASE + 178)
#define S3MA_CORESIGHT3_IRQ   					(S3MA_IRQ_BASE + 179)
/* IRQ 180 */
#define S3MA_USB0_EP0_IRQ   					(S3MA_IRQ_BASE + 180)
#define S3MA_USB0_EP1_IRQ   					(S3MA_IRQ_BASE + 181)
#define S3MA_USB0_EP2_IRQ   					(S3MA_IRQ_BASE + 182)
#define S3MA_USB0_EP3_IRQ   					(S3MA_IRQ_BASE + 183)
#define S3MA_USB0_EP4_IRQ   					(S3MA_IRQ_BASE + 184)
#define S3MA_USB0_EP5_IRQ   					(S3MA_IRQ_BASE + 185)
#define S3MA_USB0_EP6_IRQ   					(S3MA_IRQ_BASE + 186)
#define S3MA_USB0_EP7_IRQ   					(S3MA_IRQ_BASE + 187)
#define S3MA_USB0_EP8_IRQ   					(S3MA_IRQ_BASE + 188)
#define S3MA_USB0_EP9_IRQ   					(S3MA_IRQ_BASE + 189)
/* IRQ 190 */
#define S3MA_USB0_EP10_IRQ   					(S3MA_IRQ_BASE + 190)
#define S3MA_USB0_EP16_IRQ   					(S3MA_IRQ_BASE + 191)
#define S3MA_USB0_EP17_IRQ   					(S3MA_IRQ_BASE + 192)
#define S3MA_USB0_EP18_IRQ   					(S3MA_IRQ_BASE + 193)
#define S3MA_USB0_EP19_IRQ   					(S3MA_IRQ_BASE + 194)
#define S3MA_USB0_EP20_IRQ   					(S3MA_IRQ_BASE + 195)
#define S3MA_USB0_EP21_IRQ   					(S3MA_IRQ_BASE + 196)
#define S3MA_USB0_EP22_IRQ   					(S3MA_IRQ_BASE + 197)
#define S3MA_USB0_EP23_IRQ   					(S3MA_IRQ_BASE + 198)
#define S3MA_USB0_EP24_IRQ   					(S3MA_IRQ_BASE + 199)
/* IRQ 200 */
#define S3MA_USB0_EP25_IRQ   					(S3MA_IRQ_BASE + 200)
#define S3MA_USB0_EP26_IRQ   					(S3MA_IRQ_BASE + 201)
#define S3MA_USB1_EP0_IRQ   					(S3MA_IRQ_BASE + 202)
#define S3MA_USB1_EP1_IRQ   					(S3MA_IRQ_BASE + 203)
#define S3MA_USB1_EP2_IRQ   					(S3MA_IRQ_BASE + 204)
#define S3MA_USB1_EP3_IRQ   					(S3MA_IRQ_BASE + 205)
#define S3MA_USB1_EP4_IRQ   					(S3MA_IRQ_BASE + 206)
#define S3MA_USB1_EP5_IRQ   					(S3MA_IRQ_BASE + 207)
#define S3MA_USB1_EP6_IRQ   					(S3MA_IRQ_BASE + 208)
#define S3MA_USB1_EP7_IRQ   					(S3MA_IRQ_BASE + 209)
/* IRQ 210 */
#define S3MA_USB1_EP8_IRQ   					(S3MA_IRQ_BASE + 210)
#define S3MA_USB1_EP9_IRQ   					(S3MA_IRQ_BASE + 211)
#define S3MA_USB1_EP10_IRQ   					(S3MA_IRQ_BASE + 212)
#define S3MA_USB1_EP16_IRQ   					(S3MA_IRQ_BASE + 213)
#define S3MA_USB1_EP17_IRQ   					(S3MA_IRQ_BASE + 214)
#define S3MA_USB1_EP18_IRQ   					(S3MA_IRQ_BASE + 215)
#define S3MA_USB1_EP19_IRQ   					(S3MA_IRQ_BASE + 216)
#define S3MA_USB1_EP20_IRQ   					(S3MA_IRQ_BASE + 217)
#define S3MA_USB1_EP21_IRQ   					(S3MA_IRQ_BASE + 218)
#define S3MA_USB1_EP22_IRQ   					(S3MA_IRQ_BASE + 219)
/* IRQ 220 */
#define S3MA_USB1_EP23_IRQ   					(S3MA_IRQ_BASE + 220)
#define S3MA_USB1_EP24_IRQ   					(S3MA_IRQ_BASE + 221)
#define S3MA_USB1_EP25_IRQ   					(S3MA_IRQ_BASE + 222)
#define S3MA_USB1_EP26_IRQ   					(S3MA_IRQ_BASE + 223)


#endif /* S3MA_IRQS_H_ */
