#include <genesis.h>
#include <resources.h>

// States:
// 0 - Title Screen
// 1 - Cutscene (cutscene_num will be set to the cutscene number)
// 2 - Game
int state = 0;
// Cutscenes:
// 0 - Grandpa opening
int cutscene_num = 0;
//miscellaneous variables to be used for cutscenes
int cutscene_var1 = 0;
int cutscene_var2 = 0;
int cutscene_var3 = 0;
int cutscene_var4 = 0;
Sprite *cutscene_sprite1;


void loadCutscene(int cutsceneNum) {
    state = 1;
    cutscene_num = cutsceneNum;
    cutscene_var1 = 0;
    cutscene_var2 = 0;
    cutscene_var3 = 0;
    cutscene_var4 = 0;
    switch (cutsceneNum) {
        case 0:
            //grandpa opening
            //cutscene_var1 -> grandpa state countdown
            //cutscene_var2 -> 
            //cutscene_var3 -> image state (bit 0 -> 0 = grandpa1, 1 = grandpa2, bit 1 -> grandpa mouth open/hand moving, bits 2 & 3 -> fireplace state (one of 4 states), bit 4 -> has letter?)

            XGM_setLoopNumber(0);
            XGM_startPlay(&grandpas_theme);

            VDP_setPalette(PAL1, grandpa1.palette->data);
            cutscene_var3 = 128; //set bit 7 to 1

            break;
    }
}

int main(u16 hard) {

    loadCutscene(0);
    SPR_init();

    while (TRUE) {
        if (state == 1) {
            if (cutscene_num == 0) {
                //grandpa opening
                //cutscene_var1 -> countup timer
                //cutscene_var2 -> image state (bit 0 -> 0 = grandpa1, 1 = grandpa2, bit 1 -> grandpa mouth open/hand moving, bit 2 -> has letter?)
                //cutscene_var3 -> text state (bits 0-4 -> text state, bits 5 & 6 -> text fade, bit 7 -> currently fading out?, bit 8 -> currently fading in?)

                cutscene_var1++;

                if (cutscene_var1 % 20 == 0) {
                    //check bit 0 of cutscene_var3
                    if (cutscene_var2 & 1) {
                        //draw the tilemap for grandpa2
                        VDP_drawImageEx(BG_B, &grandpa2, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, TILE_USERINDEX), 0, 0, FALSE, CPU);
                        cutscene_var2 &= ~1;
                    } else {
                        //draw the tilemap for grandpa1
                        VDP_drawImageEx(BG_B, &grandpa1, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, TILE_USERINDEX), 0, 0, FALSE, CPU);
                        cutscene_var2 |= 1;
                    }
                }
                //update text
                if ((cutscene_var1 % 20) == 0) {
                    if (cutscene_var3 & 128) {
                        //text is fading out, check the state
                        switch ((cutscene_var3 >> 5) & 3) {
                            case 0:
                                PAL_setColor(15, RGB24_TO_VDPCOLOR(0x000000));
                                break;
                            case 1:
                                PAL_setColor(15, RGB24_TO_VDPCOLOR(0x404040));
                                break;
                            case 2:
                                PAL_setColor(15, RGB24_TO_VDPCOLOR(0xC0C0C0));
                                break;
                            case 3:
                                PAL_setColor(15, RGB24_TO_VDPCOLOR(0xFFFFFF));
                                break;
                        }
                        //decrease the state (if it's not already 0)
                        if (((cutscene_var3 >> 5) & 3) != 0) {
                            cutscene_var3 -= 32;
                        }
                    } else if (cutscene_var3 & 256) {
                        //text could be fading in, check the state
                        switch (((cutscene_var3 >> 5) & 3)) {
                            case 0:
                                PAL_setColor(15, RGB24_TO_VDPCOLOR(0x000000));
                                break;
                            case 1:
                                PAL_setColor(15, RGB24_TO_VDPCOLOR(0x404040));
                                break;
                            case 2:
                                PAL_setColor(15, RGB24_TO_VDPCOLOR(0xC0C0C0));
                                break;
                            case 3:
                                PAL_setColor(15, RGB24_TO_VDPCOLOR(0xFFFFFF));
                                break;
                        }
                        //increase the state (if it's not already 3)
                        if (((cutscene_var3 >> 5) & 3) != 3) {
                            cutscene_var3 += 32;
                        } else {
                            cutscene_var3 &= ~256;
                        }
                    }
                }
                //if the text fade is zero, increase the text (bit 0-4 of cutscene_var3)
                if ((((cutscene_var3 >> 5) & 3) == 0) && ((cutscene_var1 % 20) == 0)) {
                    cutscene_var3++;
                    //clear plane A and update the text
                    VDP_clearPlane(BG_A, TRUE);
                    //depending on the text state, draw a different text
                    switch (cutscene_var3 & 31) {
                        case 1:
                            VDP_drawText("Grandpa! <filler text to make sure it's seen!>", 0, 0);
                            break;
                        default:
                            VDP_drawText("problem", 0, 0);
                            break;
                    }
                    cutscene_var3 &= ~128;
                    //set the fade state to 0
                    cutscene_var3 &= ~96;
                    //set the fade in state
                    cutscene_var3 |= 256;
                }

                
                //depending on the timer state, start fading out the text
                if (cutscene_var1 == 240) {
                    cutscene_var3 |= 128;
                }

            }
        }

        SPR_update();
        SYS_doVBlankProcess();
    }
}