struct lcd_controller s3c2440_lcd_controller = {
	.init    = s3c2440_lcd_controller_init;
	.enalbe  = s3c2440_lcd_controller_enalbe;
	.disable = s3c2440_lcd_controller_disable;
};