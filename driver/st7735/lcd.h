#ifndef ST7735_H__
#define ST7735_H__

#define ST7735_NOP                          0x00U  /* No Operation: NOP                           */
#define ST7735_SW_RESET                     0x01U  /* Software reset: SWRESET                     */
#define ST7735_READ_ID                      0x04U  /* Read Display ID: RDDID                      */
#define ST7735_READ_STATUS                  0x09U  /* Read Display Statu: RDDST                   */
#define ST7735_READ_POWER_MODE              0x0AU  /* Read Display Power: RDDPM                   */
#define ST7735_READ_MADCTL                  0x0BU  /* Read Display: RDDMADCTL                     */
#define ST7735_READ_PIXEL_FORMAT            0x0CU  /* Read Display Pixel: RDDCOLMOD               */  
#define ST7735_READ_IMAGE_MODE              0x0DU  /* Read Display Image: RDDIM                   */
#define ST7735_READ_SIGNAL_MODE             0x0EU  /* Read Display Signal: RDDSM                  */           
#define ST7735_SLEEP_IN                     0x10U  /* Sleep in & booster off: SLPIN               */ 
#define ST7735_SLEEP_OUT                    0x11U  /* Sleep out & booster on: SLPOUT              */
#define ST7735_PARTIAL_DISPLAY_ON           0x12U  /* Partial mode on: PTLON                      */ 
#define ST7735_NORMAL_DISPLAY_OFF           0x13U  /* Partial off (Normal): NORON                 */
#define ST7735_DISPLAY_INVERSION_OFF        0x20U  /* Display inversion off: INVOFF               */
#define ST7735_DISPLAY_INVERSION_ON         0x21U  /* Display inversion on: INVON                 */
#define ST7735_GAMMA_SET                    0x26U  /* Gamma curve select: GAMSET                  */
#define ST7735_DISPLAY_OFF                  0x28U  /* Display off: DISPOFF                        */
#define ST7735_DISPLAY_ON                   0x29U  /* Display on: DISPON                          */
#define ST7735_CASET                        0x2AU  /* Column address set: CASET                   */ 
#define ST7735_RASET                        0x2BU  /* Row address set: RASET                      */
#define ST7735_WRITE_RAM                    0x2CU  /* Memory write: RAMWR                         */  
#define ST7735_RGBSET                       0x2DU  /* LUT for 4k,65k,262k color: RGBSET           */
#define ST7735_READ_RAM                     0x2EU  /* Memory read: RAMRD                          */
#define ST7735_PTLAR                        0x30U  /* Partial start/end address set: PTLAR        */ 
#define ST7735_TE_LINE_OFF                  0x34U  /* Tearing effect line off: TEOFF              */ 
#define ST7735_TE_LINE_ON                   0x35U  /* Tearing effect mode set & on: TEON          */ 
#define ST7735_MADCTL                       0x36U  /* Memory data access control: MADCTL          */ 
#define ST7735_IDLE_MODE_OFF                0x38U  /* Idle mode off: IDMOFF                       */ 
#define ST7735_IDLE_MODE_ON                 0x39U  /* Idle mode on: IDMON                         */ 
#define ST7735_COLOR_MODE                   0x3AU  /* Interface pixel format: COLMOD              */
#define ST7735_FRAME_RATE_CTRL1             0xB1U  /* In normal mode (Full colors): FRMCTR1       */
#define ST7735_FRAME_RATE_CTRL2             0xB2U  /* In Idle mode (8-colors): FRMCTR2            */   
#define ST7735_FRAME_RATE_CTRL3             0xB3U  /* In partial mode + Full colors: FRMCTR3      */
#define ST7735_FRAME_INVERSION_CTRL         0xB4U  /* Display inversion control: INVCTR           */
#define ST7735_DISPLAY_SETTING              0xB6U  /* Display function setting                    */
#define ST7735_PWR_CTRL1                    0xC0U  /* Power control setting: PWCTR1               */ 
#define ST7735_PWR_CTRL2                    0xC1U  /* Power control setting: PWCTR2               */ 
#define ST7735_PWR_CTRL3                    0xC2U  /* In normal mode (Full colors): PWCTR3        */
#define ST7735_PWR_CTRL4                    0xC3U  /* In Idle mode (8-colors): PWCTR4             */ 
#define ST7735_PWR_CTRL5                    0xC4U  /* In partial mode + Full colors: PWCTR5       */
#define ST7735_VCOMH_VCOML_CTRL1            0xC5U  /* VCOM control 1: VMCTR1                      */ 
#define ST7735_VMOF_CTRL                    0xC7U  /* Set VCOM offset control: VMOFCTR            */ 
#define ST7735_WRID2                        0xD1U  /* Set LCM version code: WRID2                 */ 
#define ST7735_WRID3                        0xD2U  /* Customer Project code: WRID3                */ 
#define ST7735_NV_CTRL1                     0xD9U  /* NVM control status: NVCTR1                  */
#define ST7735_READ_ID1                     0xDAU  /* Read ID1: RDID1                             */ 
#define ST7735_READ_ID2                     0xDBU  /* Read ID2: RDID2                             */ 
#define ST7735_READ_ID3                     0xDCU  /* Read ID3: RDID3                             */ 
#define ST7735_NV_CTRL2                     0xDEU  /* NVM Read Command: NVCTR2                    */ 
#define ST7735_NV_CTRL3                     0xDFU  /* NVM Write Command: NVCTR3                   */
#define ST7735_PV_GAMMA_CTRL                0xE0U  /* Set Gamma adjustment (+ polarity): GAMCTRP1 */
#define ST7735_NV_GAMMA_CTRL                0xE1U  /* Set Gamma adjustment (- polarity): GAMCTRN1 */
#define ST7735_EXT_CTRL                     0xF0U  /* Extension command control                   */
#define ST7735_PWR_CTRL6                    0xFCU  /* In partial mode + Idle mode: PWCTR6         */
#define ST7735_VCOM4_LEVEL                  0xFFU  /* VCOM 4 level control   */

#define MADCTL_MY 0x80  ///< Bottom to top
#define MADCTL_MX 0x40  ///< Right to left
#define MADCTL_MV 0x20  ///< Reverse Mode
#define MADCTL_ML 0x10  ///< LCD refresh Bottom to top
#define MADCTL_RGB 0x00 ///< Red-Green-Blue pixel order
#define MADCTL_BGR 0x08 ///< Blue-Green-Red pixel order
#define MADCTL_MH 0x04  ///< LCD refresh right to left

#define FRAME_8_B                       0
#define FRAME_16_B                      1 << 1

#endif