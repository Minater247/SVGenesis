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

//later this will be loaded from SRAM, but for now it's just a placeholder
int characterdata = 0b0000000000000001;
//characterdata bits:
// 0: gender (0 -> male, 1 -> female)
// 1-31: unused for now


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
                //cutscene_var3 -> text state

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
                
                //depending on the timer state, start fading out the text
                if (cutscene_var1 == 110) {
                    VDP_clearPlane(BG_A, TRUE);
                    //depending on whether the player is male or female, draw the appropriate text (...and for my very special grand[son/daughter]:)
                    if (characterdata & 1) {
                        VDP_drawText("...and for my very special", 7, 22);
                        VDP_drawText("granddaughter:", 13, 23);
                    } else {
                        VDP_drawText("...and for my very special grandson:", 2, 22);
                    }
                }

            }
        }

        SPR_update();
        SYS_doVBlankProcess();
    }
}