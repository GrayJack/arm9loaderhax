#include <stdint.h>
#include <stddef.h>
#include <ctr11/ctr_pxi.h>

#define BRIGHTNESS 0x39
#define FB_TOP_LEFT 0x18300000
#define FB_TOP_RIGHT 0x18300000
#define FB_BOTTOM 0x18346500

void ctr_libctr11_init(void);
void ctr_libctr11_init(void)
{
	ctr_pxi_change_base((volatile uint32_t*)0x10163000);
	//Override the default, we want no default console initialization
}

static inline void regSet(void);

static inline void flush_pxi(void)
{
	while (!ctr_pxi_receive_empty_status())
	{
		uint32_t data;
		ctr_pxi_pop(&data);
	}
}

int main()
{
	ctr_pxi_set_enabled(true);
	ctr_pxi_fifo_send_clear();

	uint32_t isScreenInit;
	while (ctr_pxi_receive_empty_status());
	ctr_pxi_pop(&isScreenInit);
	if (isScreenInit)
		regSet();
	ctr_pxi_push(1);

	// Wait for entry to be set

	void (*volatile *brahma_entry)(int argc, char *argv[]) = (void*)0x1FFFFFF8;
	void (*volatile *k11_entry)(int argc, char *argv[]) = (void*)0x1FFFFFFC;
	//void (*volatile *core1_entry)(int argc, char *argv[]) = (void*)0x1FFFFFDC;
	// Reset the entry
	*brahma_entry = *k11_entry = NULL;

	// Wait for entry to be set
	while(!*brahma_entry && !*k11_entry && ctr_pxi_receive_empty_status());

	// Jump
	if (*brahma_entry)
		(*brahma_entry)(0, NULL);
	else if (*k11_entry)
		(*k11_entry)(0, NULL);
	else//ctr_pxi_receive_empty_status()
	{
		void (*entry)(int argc, char *argv[]);
		int argc;
		char **argv;
		ctr_pxi_pop((uint32_t*)&entry);
		while(ctr_pxi_receive_empty_status());
		ctr_pxi_pop((uint32_t*)&argc);
		while(ctr_pxi_receive_empty_status());
		ctr_pxi_pop((uint32_t*)&argv);

		flush_pxi();
		entry(argc, argv);
	}
	//else if (*core1_entry)
	//	(*core1_entry)(0, NULL);

	return 0;
}

#define PDN_GPU_CNT (*(volatile uint32_t*)0x10141200)
#define LCD_REG(offset) (*((volatile uint32_t*)(0x10202000 + (offset))))
#define LCD_TOP_CONF_REG(offset) (*((volatile uint32_t*)(0x10202200 + (offset))))
#define LCD_BOT_CONF_REG(offset) (*((volatile uint32_t*)(0x10202A00 + (offset))))

#define LCD_TOP_CONF_BRIGHTNESS LCD_TOP_CONF_REG(0x40)
#define LCD_BOT_CONF_BRIGHTNESS LCD_BOT_CONF_REG(0x40)

#define PDC0_FRAMEBUFFER_SETUP_REG(offset) (*((volatile uint32_t*)(0x10400400 + (offset))))
#define PDC1_FRAMEBUFFER_SETUP_REG(offset) (*((volatile uint32_t*)(0x10400500 + (offset))))

#define PDC0_FRAMEBUFFER_SETUP_DIMS PDC0_FRAMEBUFFER_SETUP_REG(0x5C)
#define PDC0_FRAMEBUFFER_SETUP_FBA_ADDR_1 PDC0_FRAMEBUFFER_SETUP_REG(0x68)
#define PDC0_FRAMEBUFFER_SETUP_FBA_ADDR_2 PDC0_FRAMEBUFFER_SETUP_REG(0x6C)
#define PDC0_FRAMEBUFFER_SETUP_FB_FORMAT PDC0_FRAMEBUFFER_SETUP_REG(0x70)
#define PDC0_FRAMEBUFFER_SETUP_FB_SELECT PDC0_FRAMEBUFFER_SETUP_REG(0x78)
#define PDC0_FRAMEBUFFER_SETUP_DISCO PDC0_FRAMEBUFFER_SETUP_REG(0x84)
#define PDC0_FRAMEBUFFER_SETUP_FB_STRIDE PDC0_FRAMEBUFFER_SETUP_REG(0x90)
#define PDC0_FRAMEBUFFER_SETUP_FBB_ADDR_1 PDC0_FRAMEBUFFER_SETUP_REG(0x94)
#define PDC0_FRAMEBUFFER_SETUP_FBB_ADDR_2 PDC0_FRAMEBUFFER_SETUP_REG(0x98)

#define PDC1_FRAMEBUFFER_SETUP_DIMS PDC1_FRAMEBUFFER_SETUP_REG(0x5C)
#define PDC1_FRAMEBUFFER_SETUP_FBA_ADDR_1 PDC1_FRAMEBUFFER_SETUP_REG(0x68)
#define PDC1_FRAMEBUFFER_SETUP_FBA_ADDR_2 PDC1_FRAMEBUFFER_SETUP_REG(0x6C)
#define PDC1_FRAMEBUFFER_SETUP_FB_FORMAT PDC1_FRAMEBUFFER_SETUP_REG(0x70)
#define PDC1_FRAMEBUFFER_SETUP_FB_SELECT PDC1_FRAMEBUFFER_SETUP_REG(0x78)
#define PDC1_FRAMEBUFFER_SETUP_DISCO PDC1_FRAMEBUFFER_SETUP_REG(0x84)
#define PDC1_FRAMEBUFFER_SETUP_FB_STRIDE PDC1_FRAMEBUFFER_SETUP_REG(0x90)
#define PDC1_FRAMEBUFFER_SETUP_FBB_ADDR_1 PDC1_FRAMEBUFFER_SETUP_REG(0x94)
#define PDC1_FRAMEBUFFER_SETUP_FBB_ADDR_2 PDC1_FRAMEBUFFER_SETUP_REG(0x98)


static inline void regSet(void)
{
	PDN_GPU_CNT = 0x1007F; //bit0: Enable GPU regs 0x10400000+, bit16 turn on LCD backlight?
	LCD_REG(0x14) = 0x00000001; //UNKNOWN register, maybe LCD related? 0x10202000
	LCD_REG(0xC) &= 0xFFFEFFFE; //UNKNOWN register, maybe LCD related?

	LCD_TOP_CONF_BRIGHTNESS = BRIGHTNESS;
	LCD_BOT_CONF_BRIGHTNESS = BRIGHTNESS;
	LCD_TOP_CONF_REG(0x44) = 0x1023E; //unknown
	LCD_BOT_CONF_REG(0x44) = 0x1023E; //unknown

	// Top screen
	PDC0_FRAMEBUFFER_SETUP_REG(0x00) = 0x000001c2; //unknown
	PDC0_FRAMEBUFFER_SETUP_REG(0x04) = 0x000000d1; //unknown
	PDC0_FRAMEBUFFER_SETUP_REG(0x08) = 0x000001c1;
	PDC0_FRAMEBUFFER_SETUP_REG(0x0c) = 0x000001c1;
	PDC0_FRAMEBUFFER_SETUP_REG(0x10) = 0x00000000;
	PDC0_FRAMEBUFFER_SETUP_REG(0x14) = 0x000000cf;
	PDC0_FRAMEBUFFER_SETUP_REG(0x18) = 0x000000d1;
	PDC0_FRAMEBUFFER_SETUP_REG(0x1c) = 0x01c501c1;
	PDC0_FRAMEBUFFER_SETUP_REG(0x20) = 0x00010000;
	PDC0_FRAMEBUFFER_SETUP_REG(0x24) = 0x0000019d;
	PDC0_FRAMEBUFFER_SETUP_REG(0x28) = 0x00000002;
	PDC0_FRAMEBUFFER_SETUP_REG(0x2c) = 0x00000192;
	PDC0_FRAMEBUFFER_SETUP_REG(0x30) = 0x00000192;
	PDC0_FRAMEBUFFER_SETUP_REG(0x34) = 0x00000192;
	PDC0_FRAMEBUFFER_SETUP_REG(0x38) = 0x00000001;
	PDC0_FRAMEBUFFER_SETUP_REG(0x3c) = 0x00000002;
	PDC0_FRAMEBUFFER_SETUP_REG(0x40) = 0x01960192;
	PDC0_FRAMEBUFFER_SETUP_REG(0x44) = 0x00000000;
	PDC0_FRAMEBUFFER_SETUP_REG(0x48) = 0x00000000;
	PDC0_FRAMEBUFFER_SETUP_DIMS = (240u << 16) | (400u);
	PDC0_FRAMEBUFFER_SETUP_REG(0x60) = 0x01c100d1;
	PDC0_FRAMEBUFFER_SETUP_REG(0x64) = 0x01920002;
	PDC0_FRAMEBUFFER_SETUP_FBA_ADDR_1 = 0x18300000;
	PDC0_FRAMEBUFFER_SETUP_FB_FORMAT = 0x80341;
	PDC0_FRAMEBUFFER_SETUP_REG(0x74) = 0x00010501;
	PDC0_FRAMEBUFFER_SETUP_FB_SELECT = 0;
	PDC0_FRAMEBUFFER_SETUP_FB_STRIDE = 240u * 3;
	PDC0_FRAMEBUFFER_SETUP_REG(0x9C) = 0x00000000;

	// Disco register
	for(volatile uint32_t i = 0; i < 256; i++)
		PDC0_FRAMEBUFFER_SETUP_DISCO = 0x10101 * i;

	// Bottom screen
	PDC1_FRAMEBUFFER_SETUP_REG(0x00) = 0x000001c2;
	PDC1_FRAMEBUFFER_SETUP_REG(0x04) = 0x000000d1;
	PDC1_FRAMEBUFFER_SETUP_REG(0x08) = 0x000001c1;
	PDC1_FRAMEBUFFER_SETUP_REG(0x0c) = 0x000001c1;
	PDC1_FRAMEBUFFER_SETUP_REG(0x10) = 0x000000cd;
	PDC1_FRAMEBUFFER_SETUP_REG(0x14) = 0x000000cf;
	PDC1_FRAMEBUFFER_SETUP_REG(0x18) = 0x000000d1;
	PDC1_FRAMEBUFFER_SETUP_REG(0x1c) = 0x01c501c1;
	PDC1_FRAMEBUFFER_SETUP_REG(0x20) = 0x00010000;
	PDC1_FRAMEBUFFER_SETUP_REG(0x24) = 0x0000019d;
	PDC1_FRAMEBUFFER_SETUP_REG(0x28) = 0x00000052;
	PDC1_FRAMEBUFFER_SETUP_REG(0x2c) = 0x00000192;
	PDC1_FRAMEBUFFER_SETUP_REG(0x30) = 0x00000192;
	PDC1_FRAMEBUFFER_SETUP_REG(0x34) = 0x0000004f;
	PDC1_FRAMEBUFFER_SETUP_REG(0x38) = 0x00000050;
	PDC1_FRAMEBUFFER_SETUP_REG(0x3c) = 0x00000052;
	PDC1_FRAMEBUFFER_SETUP_REG(0x40) = 0x01980194;
	PDC1_FRAMEBUFFER_SETUP_REG(0x44) = 0x00000000;
	PDC1_FRAMEBUFFER_SETUP_REG(0x48) = 0x00000011;
	PDC1_FRAMEBUFFER_SETUP_DIMS = (240u << 16) | 320u;
	PDC1_FRAMEBUFFER_SETUP_REG(0x60) = 0x01c100d1;
	PDC1_FRAMEBUFFER_SETUP_REG(0x64) = 0x01920052;
	PDC1_FRAMEBUFFER_SETUP_FBA_ADDR_1 = 0x18300000 + 0x46500;
	PDC1_FRAMEBUFFER_SETUP_FB_FORMAT = 0x80301;
	PDC1_FRAMEBUFFER_SETUP_REG(0x74) = 0x00010501;
	PDC1_FRAMEBUFFER_SETUP_FB_SELECT = 0;
	PDC1_FRAMEBUFFER_SETUP_FB_STRIDE = 240u * 3;
	PDC1_FRAMEBUFFER_SETUP_REG(0x9C) = 0x00000000;

	// Disco register
	for(volatile uint32_t i = 0; i < 256; i++)
		PDC1_FRAMEBUFFER_SETUP_DISCO = 0x10101 * i;

	PDC0_FRAMEBUFFER_SETUP_FBA_ADDR_1 = FB_TOP_LEFT;
	PDC0_FRAMEBUFFER_SETUP_FBA_ADDR_2 = FB_TOP_LEFT;
	PDC0_FRAMEBUFFER_SETUP_FBB_ADDR_1 = FB_TOP_RIGHT;
	PDC0_FRAMEBUFFER_SETUP_FBB_ADDR_2 = FB_TOP_RIGHT;
	PDC1_FRAMEBUFFER_SETUP_FBA_ADDR_1 = FB_BOTTOM;
	PDC1_FRAMEBUFFER_SETUP_FBA_ADDR_2 = FB_BOTTOM;

	}

