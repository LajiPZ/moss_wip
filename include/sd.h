#ifndef __SD_H__
#define __SD_H__


typedef struct {
    uint16_t attribute;
    uint16_t length;
    uint32_t paddr;
} adma2_desc_entry;

#define SDREG(reg) (*((volatile uint16_t *)(KSEG1 + reg)))
#define SDREG_8(reg) (*((volatile uint8_t *)(KSEG1 + reg)))
void sd_init();
void sd_send_cmd(int index, uint16_t argh, uint16_t argl, int *res);
void sd_send_cmd18(u_int sd_addr);
void sd_send_cmd25(u_int sd_addr);
#endif 