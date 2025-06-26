#include <megasoc.h>
#include <mmu.h>
#include <printk.h>
#include <sd.h>

int rca;

void sd_send_cmd(int index, uint16_t argh, uint16_t argl, int *res) {
    uint16_t cmd = index << 8;
    // Set arguments
    SDREG(MEGASOC_SD_ARGR_H) = argh;
    SDREG(MEGASOC_SD_ARGR_L) = argl;
    // Set and send cmd
    switch(index) {
        case 13: cmd |= (0b10 << 0) | (1 << 3) | (1 << 4); break; 
        case 0: break;
        case 8: cmd |= (0b10 << 0) | (1 << 3) | (1 << 4); break; 
        case 55: cmd |= (0b10 << 0) | (1 << 3) | (1 << 4); break;
        case 41: cmd |= (0b10 << 0); break;
        case 2: cmd |= (0b01 << 0) | (1 << 3); break;
        case 23: case 16: case 3: cmd |= (0b10 << 0) | (1 << 3) | (1 << 4); break;
        case 7: cmd |= (0b11 << 0) | (1 << 3) | (1 << 4); break;
    }
    // debugf("Sending CMD%d: %x\n",index,cmd);
    // debugf("ARGS: %x %x\n",SDREG(MEGASOC_SD_ARGR_H), SDREG(MEGASOC_SD_ARGR_L));
    SDREG(MEGASOC_SD_CMDR)= cmd;
    // Wait
    while(!SDREG(MEGASOC_SD_NISR)) {};
    // Check err
    if (SDREG(MEGASOC_SD_EISR)) {
        // debugf("CMD%d ERR: %d\n",index,SDREG(MEGASOC_SD_EISR));
        return;
    }
    // Return
    if (res) {
        for (int i = 0; i < 8; i++) {
            res[i] = SDREG(MEGASOC_SD_RES0 + 0x2 * i);
        }
    }
    // Reset interruputs
    // TODO: This is DUMB; see if it works
    SDREG(MEGASOC_SD_NISR) = 0xffff;
}

void sd_send_cmd18(u_int sd_addr) {
    uint16_t cmd = 18 << 8;
    // Set arguments
    SDREG(MEGASOC_SD_ARGR_H) = ((uint16_t)(sd_addr >> 16));
    SDREG(MEGASOC_SD_ARGR_L) = ((uint16_t)(sd_addr & 0xffff));
    // Set flags
    cmd |= (0b10 << 0) | (1 << 3) | (1 << 4) | (1 << 5); 
    // debugf("Sending CMD18: %x\n",cmd);
    // debugf("ARGS: %x %x\n",SDREG(MEGASOC_SD_ARGR_H), SDREG(MEGASOC_SD_ARGR_L));
    SDREG(MEGASOC_SD_CMDR)= cmd;
    // Wait
    while(!(SDREG(MEGASOC_SD_NISR) & (0x2))) {
    };
    // Check err
    if (SDREG(MEGASOC_SD_EISR) | SDREG(MEGASOC_SD_ADMAESR)) {
        // debugf("CMD18 ERR: %d\n",SDREG(MEGASOC_SD_EISR));
        // debugf("CMD18 ADMA_ERR: %d\n",SDREG(MEGASOC_SD_ADMAESR));
        return;
    }
    // Reset interruputs
    // TODO: This is DUMB; see if it works
    SDREG(MEGASOC_SD_NISR) = 0xffff;
}

void sd_send_cmd25(u_int sd_addr) {
    uint16_t cmd = 25 << 8;
    // Set arguments
    SDREG(MEGASOC_SD_ARGR_H) = ((uint16_t)(sd_addr >> 16));
    SDREG(MEGASOC_SD_ARGR_L) = ((uint16_t)(sd_addr & 0xffff));
    // Set flags
    cmd |= (0b10 << 0) | (1 << 3) | (1 << 4) | (1 << 5); 
    // debugf("Sending CMD25: %x\n",cmd);
    // debugf("ARGS: %x %x\n",SDREG(MEGASOC_SD_ARGR_H), SDREG(MEGASOC_SD_ARGR_L));
    SDREG(MEGASOC_SD_CMDR)= cmd;
    // Wait
    while(!(SDREG(MEGASOC_SD_NISR) & (0x2))) {
    };
    // Check err
    if (SDREG(MEGASOC_SD_EISR) | SDREG(MEGASOC_SD_ADMAESR)) {
        // debugf("CMD18 ERR: %d\n",SDREG(MEGASOC_SD_EISR));
        // debugf("CMD18 ADMA_ERR: %d\n",SDREG(MEGASOC_SD_ADMAESR));
        return;
    }
    // Reset interruputs
    // TODO: This is DUMB; see if it works
    SDREG(MEGASOC_SD_NISR) = 0xffff;
}

void sd_intr_init() {
    SDREG(MEGASOC_SD_NISER) = (
        1 << 0 | // CMD done
        1 << 1 | // Transfer done
        1 << 3 // DMA intr
    );
    SDREG(MEGASOC_SD_EISER) = (
        1 << 0 | 1 << 1 | // CMD timeout/CRC
        1 << 4 | 1 << 5 | // DAT timeout/CRC
        1 << 9 // ADMA err
    );
}

void sd_clk_init() {
    uint8_t base_clk = (uint8_t)(SDREG(MEGASOC_SD_CAPR_L16) >> 8);
    SDREG(MEGASOC_SD_CCR) = (
        0x00 << 8 | // base clk, no div
        0x1 << 0 // Internal clk enable
    );
    while (!(SDREG(MEGASOC_SD_CCR) & 0x0002)) {} // Internal clk stable
    SDREG(MEGASOC_SD_CCR) |= 0x1 << 2; // Enable SD clk 
}

void sd_init() {
    int res[8] = {0};
    sd_clk_init();
    sd_intr_init();
    // Send CMD0
    sd_send_cmd(0, 0, 0, NULL);
    // Send CMD8
    int target_voltage = 0x1AA;
    sd_send_cmd(8, 0, target_voltage, &res);
    // printk("CMD8 returned %x\n", res[0]);
    if (target_voltage != res[0]) panic("Not implemented: No response after CMD8");
    // Then, response is valid, send ACMD41
    do {
        sd_send_cmd(55,0,0,res);
        sd_send_cmd(
            41,
            0x4030, // HCS = 1; voltage -> 3.3v; S18R = 0
            0,
            res
        );
    } while(!(res[1] & (1 << 15)));
    int ccs = (res[1] & (1 << 14)) >> 14;
    // debugf("CCS: %x\n", ccs);
    // S18R = 0; directly to CMD2/CMD3
    // TODO: Is that it?
    sd_send_cmd(2,0,0,res);
    /*
    for (int i = 0; i < 8; i++) {
        debugf("CMD2 res%d: %x\n",i,res[i]);
    }
    */
    sd_send_cmd(3,0,0,res);
    rca = res[1];
    // debugf("RCA: %x\n", rca);
    
    for (int i = 0; i < 8; i++) {
        // debugf("CMD3 res%d: %x\n",i,res[i]);
    }
    sd_send_cmd(7,rca,0,NULL); 

    // Setup ADMA2
    SDREG_8(MEGASOC_SD_HC1R) = 0x2 << 3;
}
