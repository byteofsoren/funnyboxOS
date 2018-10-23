/*
 * pong_charts.h
 *
 * Created: 2018-10-21 21:15:07
 *  Author: MDF
 */ 


#ifndef PONG_CHARTS_H_
#define PONG_CHARTS_H_

	// ball and no wall
	#define disp_leftUp      0b11111000
	#define disp_rightUp     0b11110100
	#define disp_leftDown    0b11110010
	#define disp_rightDown   0b11110001
	// ball and wall
	#define disp_WleftUp	 0b11101000    
	#define disp_WrightUp	 0b11100100    
	#define disp_WleftDown	 0b11100010    
	#define disp_WrightDown	 0b11100001    
	// no ball and wall
	#define disp_Wleft		 0b11100000    
	#define disp_Wright	     0b11100000    
	
	
 // === Balls with no wall ===
	const unsigned char charts_leftUp[8] = {
		0b00000,
		0b01000,
		0b10100,
		0b01000,
		0b00000,
		0b00000,
		0b00000,
		0b00000};
	const unsigned char charts_rightUp[8] = {
		0b00000,
		0b00010,
		0b00101,
		0b00010,
		0b00000,
		0b00000,
		0b00000,
		0b00000};
	const unsigned char charts_leftDown[8] = {
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b01000,
		0b10100,
		0b01000,
		0b00000};
	const unsigned char charts_rightDown[8] = {
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00010,
		0b00101,
		0b00010,
		0b00000};
// == walls with ball ==
	const unsigned char charts_wallLeftUP[8] = {
		0b10000,
		0b11000,
		0b10100,
		0b11000,
		0b10000,
		0b10000,
		0b10000,
		0b10000};
   const unsigned char charts_wallLeftDown[8] = {
		 0b10000,
		 0b10000,
		 0b10000,
		 0b10000,
		 0b11000,
		 0b10100,
		 0b11000,
		 0b10000};
   const unsigned char charts_wallRightUP[8] = {
		0b00001,
		0b00011,
		0b00101,
		0b00011,
		0b00001,
		0b00001,
		0b00001,
		0b00001};
   const unsigned char charts_wallRightDown[8] = {
		0b00001,
		0b00001,
		0b00001,
		0b00001,
		0b00011,
		0b00101,
		0b00011,
		0b00001};
// === Walls with do ball === 
   const unsigned char charts_wallLeft[8] = {
		0b10000,
		0b10000,
		0b10000,
		0b10000,
		0b10000,
		0b10000,
		0b10000,
		0b10000
   };
   const unsigned char charts_wallRight[8] = {
		0b00001,
		0b00001,
		0b00001,
		0b00001,
		0b00001,
		0b00001,
		0b00001,
		0b00001
   };


#endif /* PONG_CHARTS_H_ */