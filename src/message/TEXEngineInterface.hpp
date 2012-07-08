#include	"object.hpp"

#ifndef		TEXENGINE_TEX_ENGINE_INTERFACE_HPP_INCLUDED
	#define	TEXENGINE_TEX_ENGINE_INTERFACE_HPP_INCLUDED

/*
	managers need to hardcode the ids
	messages need a specific offset depending on the origin

	mesage codes and ids are as follows:

	0x0... , 0 : Reserved
	0x1... , 1 : Render Manager
	0x2... , 2 : Input Manager
	0x3... , 3 : Audio Manager
	0x4... , 4 : Physics Manager
	0x5... , 5 : UI Manager
	0x6... , 6 : Asset Manager
	0x7... , 7 : Shader Manager
	0x8... , 8 : Scene Manager
	0x9... , 9 : Log Manager
*/

	namespace	TEXEngine
	{
		namespace	Core
		{

			const unsigned long	ALL_EVENTS =										0x0000;
			const unsigned long	MESSAGE_MANAGER_KEEP_HISTORY =						0x0001;
			const unsigned long	MESSAGE_MANAGER_OBJECT_CREATED =					0x0002;
			const unsigned long	MESSAGE_MANAGER_OBJECT_DELETED =					0x0003;
			const unsigned long	FAILURE =											0x0004;
			const unsigned long	SUCESS =											0x0005;
			const unsigned long	INITIALISE =										0x0006;
			const unsigned long	EXIT =												0x0007;
			const unsigned long	UPDATE =											0x0008;
			const unsigned long	UPDATE_FAST =										0x0009;
			const unsigned long	UPDATE_MEDIUM =										0x000A;
			const unsigned long	UPDATE_SLOW =										0x000B;

			const unsigned long	RENDER_MANAGER_CHANGE_UPDATE_RATE =					0x1000;

			const unsigned long	INPUT_MANAGER_CHANGE_UPDATE_RATE =					0x2000;
			const unsigned long	INIT_KEYBOARD =										0x2001;
			const unsigned long	INIT_MOUSE =										0x2002;
		
			const unsigned long	INIT_XBOX360_CONTROLLER =							0x2005;
			const unsigned long	KEYBOARD_KEY_CANCEL_DOWN =							0x2007;
			const unsigned long	KEYBOARD_KEY_BACKSPACE_DOWN =						0x2008;
			const unsigned long	KEYBOARD_KEY_TAB_DOWN =								0x2009;
			const unsigned long	KEYBOARD_KEY_CLEAR_DOWN =							0x200C;
			const unsigned long	KEYBOARD_KEY_RETURN_DOWN =							0x200D;
			const unsigned long	KEYBOARD_KEY_SHIFT_DOWN =							0x2010;
			const unsigned long	KEYBOARD_KEY_CONTROL_DOWN =							0x2011;
			const unsigned long	KEYBOARD_KEY_ALT_DOWN =								0x2012;
			const unsigned long	KEYBOARD_KEY_PAUSE_DOWN =							0x2013;
			const unsigned long	KEYBOARD_KEY_CAPSLOCK_DOWN =						0x2014;
			const unsigned long	KEYBOARD_KEY_ESC_DOWN =								0x201B;
			const unsigned long	KEYBOARD_KEY_SPACE_DOWN =							0x2020;
			const unsigned long	KEYBOARD_KEY_PAGEUP_DOWN =							0x2021;
			const unsigned long	KEYBOARD_KEY_PAGEDOWN_DOWN =						0x2022;
			const unsigned long	KEYBOARD_KEY_END_DOWN =								0x2023;
			const unsigned long	KEYBOARD_KEY_HOME_DOWN =							0x2024;
			const unsigned long	KEYBOARD_KEY_LEFTARROW_DOWN =						0x2025;
			const unsigned long	KEYBOARD_KEY_UPARROW_DOWN =							0x2026;
			const unsigned long	KEYBOARD_KEY_RIGHTARROW_DOWN =						0x2027;
			const unsigned long	KEYBOARD_KEY_DOWNARROW_DOWN =						0x2028;
			const unsigned long	KEYBOARD_KEY_SELECT_DOWN =							0x2029;
			const unsigned long	KEYBOARD_KEY_EXECUTE_DOWN =							0x202B;
			const unsigned long	KEYBOARD_KEY_PRINTSCREEN_DOWN =						0x202C;
			const unsigned long	KEYBOARD_KEY_INSERT_DOWN =							0x202D;
			const unsigned long	KEYBOARD_KEY_DELETE_DOWN =							0x202E;
			const unsigned long	KEYBOARD_KEY_HELP_DOWN =							0x202F;
			const unsigned long	KEYBOARD_KEY_ZERO_DOWN =							0x2030;
			const unsigned long	KEYBOARD_KEY_ONE_DOWN =								0x2031;
			const unsigned long	KEYBOARD_KEY_TWO_DOWN =								0x2032;
			const unsigned long	KEYBOARD_KEY_THREE_DOWN =							0x2033;
			const unsigned long	KEYBOARD_KEY_FOUR_DOWN =							0x2034;
			const unsigned long	KEYBOARD_KEY_FIVE_DOWN =							0x2035;
			const unsigned long	KEYBOARD_KEY_SIX_DOWN =								0x2036;
			const unsigned long	KEYBOARD_KEY_SEVEN_DOWN =							0x2037;
			const unsigned long	KEYBOARD_KEY_EIGHT_DOWN =							0x2038;
			const unsigned long	KEYBOARD_KEY_NINE_DOWN =							0x2039;
			const unsigned long	KEYBOARD_KEY_A_DOWN =								0x2041;
			const unsigned long	KEYBOARD_KEY_B_DOWN =								0x2042;
			const unsigned long	KEYBOARD_KEY_C_DOWN =								0x2043;
			const unsigned long	KEYBOARD_KEY_D_DOWN =								0x2044;
			const unsigned long	KEYBOARD_KEY_E_DOWN =								0x2045;
			const unsigned long	KEYBOARD_KEY_F_DOWN =								0x2046;
			const unsigned long	KEYBOARD_KEY_G_DOWN =								0x2047;
			const unsigned long	KEYBOARD_KEY_H_DOWN =								0x2048;
			const unsigned long	KEYBOARD_KEY_I_DOWN =								0x2049;
			const unsigned long	KEYBOARD_KEY_J_DOWN =								0x204A;
			const unsigned long	KEYBOARD_KEY_K_DOWN =								0x204B;
			const unsigned long	KEYBOARD_KEY_L_DOWN =								0x204C;
			const unsigned long	KEYBOARD_KEY_M_DOWN =								0x204D;
			const unsigned long	KEYBOARD_KEY_N_DOWN =								0x204E;
			const unsigned long	KEYBOARD_KEY_O_DOWN =								0x204F;
			const unsigned long	KEYBOARD_KEY_P_DOWN =								0x2050;
			const unsigned long	KEYBOARD_KEY_Q_DOWN =								0x2051;
			const unsigned long	KEYBOARD_KEY_R_DOWN =								0x2052;
			const unsigned long	KEYBOARD_KEY_S_DOWN =								0x2053;
			const unsigned long	KEYBOARD_KEY_T_DOWN =								0x2054;
			const unsigned long	KEYBOARD_KEY_U_DOWN =								0x2055;
			const unsigned long	KEYBOARD_KEY_V_DOWN =								0x2056;
			const unsigned long	KEYBOARD_KEY_W_DOWN =								0x2057;
			const unsigned long	KEYBOARD_KEY_X_DOWN =								0x2058;
			const unsigned long	KEYBOARD_KEY_Y_DOWN =								0x2059;
			const unsigned long	KEYBOARD_KEY_Z_DOWN =								0x205A;
			const unsigned long	KEYBOARD_KEY_LEFTWINDOWS_DOWN =						0x205B;
			const unsigned long	KEYBOARD_KEY_RIGHTWINDOWS_DOWN =					0x205C;
			const unsigned long	KEYBOARD_KEY_APPLICATION_DOWN =						0x205D;
			const unsigned long	KEYBOARD_KEY_NUMPAD0_DOWN =							0x2060;
			const unsigned long	KEYBOARD_KEY_NUMPAD1_DOWN =							0x2061;
			const unsigned long	KEYBOARD_KEY_NUMPAD2_DOWN =							0x2062;
			const unsigned long	KEYBOARD_KEY_NUMPAD3_DOWN =							0x2063;
			const unsigned long	KEYBOARD_KEY_NUMPAD4_DOWN =							0x2064;
			const unsigned long	KEYBOARD_KEY_NUMPAD5_DOWN =							0x2065;
			const unsigned long	KEYBOARD_KEY_NUMPAD6_DOWN =							0x2066;
			const unsigned long	KEYBOARD_KEY_NUMPAD7_DOWN =							0x2067;
			const unsigned long	KEYBOARD_KEY_NUMPAD8_DOWN =							0x2068;
			const unsigned long	KEYBOARD_KEY_NUMPAD9_DOWN =							0x2069;
			const unsigned long	KEYBOARD_KEY_MULTIPLY_DOWN =						0x206A;
			const unsigned long	KEYBOARD_KEY_ADD_DOWN =								0x206B;
			const unsigned long	KEYBOARD_KEY_SEPARATOR_DOWN =						0x206C;
			const unsigned long	KEYBOARD_KEY_SUBTRACT_DOWN =						0x206D;
			const unsigned long	KEYBOARD_KEY_DECIMAL_DOWN =							0x206E;
			const unsigned long	KEYBOARD_KEY_DIVIDE_DOWN =							0x206F;
			const unsigned long	KEYBOARD_KEY_F1_DOWN =								0x2070;
			const unsigned long	KEYBOARD_KEY_F2_DOWN =								0x2071;
			const unsigned long	KEYBOARD_KEY_F3_DOWN =								0x2072;
			const unsigned long	KEYBOARD_KEY_F4_DOWN =								0x2073;
			const unsigned long	KEYBOARD_KEY_F5_DOWN =								0x2074;
			const unsigned long	KEYBOARD_KEY_F6_DOWN =								0x2075;
			const unsigned long	KEYBOARD_KEY_F7_DOWN =								0x2076;
			const unsigned long	KEYBOARD_KEY_F8_DOWN =								0x2077;
			const unsigned long	KEYBOARD_KEY_F9_DOWN =								0x2078;
			const unsigned long	KEYBOARD_KEY_F10_DOWN =								0x2079;
			const unsigned long	KEYBOARD_KEY_F11_DOWN =								0x207A;
			const unsigned long	KEYBOARD_KEY_F12_DOWN =								0x207B;
			const unsigned long	KEYBOARD_KEY_F13_DOWN =								0x207C;
			const unsigned long	KEYBOARD_KEY_F14_DOWN =								0x207D;
			const unsigned long	KEYBOARD_KEY_F15_DOWN =								0x207E;
			const unsigned long	KEYBOARD_KEY_F16_DOWN =								0x207F;
			const unsigned long	KEYBOARD_KEY_F17_DOWN =								0x2080;
			const unsigned long	KEYBOARD_KEY_F18_DOWN =								0x2081;
			const unsigned long	KEYBOARD_KEY_F19_DOWN =								0x2082;
			const unsigned long	KEYBOARD_KEY_F20_DOWN =								0x2083;
			const unsigned long	KEYBOARD_KEY_F21_DOWN =								0x2084;
			const unsigned long	KEYBOARD_KEY_F22_DOWN =								0x2085;
			const unsigned long	KEYBOARD_KEY_F23_DOWN =								0x2086;
			const unsigned long	KEYBOARD_KEY_F24_DOWN =								0x2087;
			const unsigned long	KEYBOARD_KEY_NUMLOCK_DOWN =							0x2090;
			const unsigned long	KEYBOARD_KEY_SCROLLLOCK_DOWN =						0x2091;
			const unsigned long	KEYBOARD_KEY_LEFTSHIFT_DOWN =						0x20A0;
			const unsigned long	KEYBOARD_KEY_RIGHTSHIFT_DOWN =						0x20A1;
			const unsigned long	KEYBOARD_KEY_LEFTCONTROL_DOWN =						0x20A2;
			const unsigned long	KEYBOARD_KEY_RIGHTCONTROL_DOWN =					0x20A3;
			const unsigned long	KEYBOARD_KEY_LEFTALT_DOWN =							0x20A4;
			const unsigned long	KEYBOARD_KEY_RIGHTALT_DOWN =						0x20A5;
			const unsigned long	KEYBOARD_KEY_SEMICOLON_DOWN =						0x20BA;
			const unsigned long	KEYBOARD_KEY_PLUS_DOWN =							0x20BB;
			const unsigned long	KEYBOARD_KEY_COMMA_DOWN =							0x20BC;
			const unsigned long	KEYBOARD_KEY_MINUS_DOWN =							0x20BD;
			const unsigned long	KEYBOARD_KEY_DOT_DOWN =								0x20BE;
			const unsigned long	KEYBOARD_KEY_FORWARDSLASH_DOWN =					0x20BF;
			const unsigned long	KEYBOARD_KEY_TILDE_DOWN =							0x20C0;
			const unsigned long	KEYBOARD_KEY_LEFTBRACKET_DOWN =						0x20DB;
			const unsigned long	KEYBOARD_KEY_BACKWARDSSLASH_DOWN =					0x20DC;
			const unsigned long	KEYBOARD_KEY_RIGHTBRACKET_DOWN =					0x20DD;
			const unsigned long	KEYBOARD_KEY_DOUBLEQUOTES_DOWN =					0x20DE;
			const unsigned long	KEYBOARD_KEY_PACKET_DOWN =							0x20E7;
			const unsigned long	KEYBOARD_KEY_ATTN_DOWN =							0x20F6;
			const unsigned long	KEYBOARD_KEY_CRSEL_DOWN =							0x20F7;
			const unsigned long	KEYBOARD_KEY_EXSEL_DOWN =							0x20F8;
			const unsigned long	KEYBOARD_KEY_EREOF_DOWN =							0x20F9;
			const unsigned long	KEYBOARD_KEY_PLAY_DOWN =							0x20FA;
			const unsigned long	KEYBOARD_KEY_ZOOM_DOWN =							0x20FB;
			const unsigned long	KEYBOARD_KEY_NONAME_DOWN =							0x20FC;
			const unsigned long	KEYBOARD_KEY_PA1_DOWN =								0x20FD;
			const unsigned long	KEYBOARD_KEY_OEMCLEAR_DOWN =						0x20FE;
			const unsigned long	KEYBOARD_KEY_CANCEL_UP =							0x2107;
			const unsigned long	KEYBOARD_KEY_BACKSPACE_UP =							0x2108;
			const unsigned long	KEYBOARD_KEY_TAB_UP =								0x2109;
			const unsigned long	KEYBOARD_KEY_CLEAR_UP =								0x210C;
			const unsigned long	KEYBOARD_KEY_RETURN_UP =							0x210D;
			const unsigned long	KEYBOARD_KEY_SHIFT_UP =								0x2110;
			const unsigned long	KEYBOARD_KEY_CONTROL_UP =							0x2111;
			const unsigned long	KEYBOARD_KEY_ALT_UP =								0x2112;
			const unsigned long	KEYBOARD_KEY_PAUSE_UP =								0x2113;
			const unsigned long	KEYBOARD_KEY_CAPSLOCK_UP =							0x2114;
			const unsigned long	KEYBOARD_KEY_ESC_UP =								0x211B;
			const unsigned long	KEYBOARD_KEY_SPACE_UP =								0x2120;
			const unsigned long	KEYBOARD_KEY_PAGEUP_UP =							0x2121;
			const unsigned long	KEYBOARD_KEY_PAGEDOWN_UP =							0x2122;
			const unsigned long	KEYBOARD_KEY_END_UP =								0x2123;
			const unsigned long	KEYBOARD_KEY_HOME_UP =								0x2124;
			const unsigned long	KEYBOARD_KEY_LEFTARROW_UP =							0x2125;
			const unsigned long	KEYBOARD_KEY_UPARROW_UP =							0x2126;
			const unsigned long	KEYBOARD_KEY_RIGHTARROW_UP =						0x2127;
			const unsigned long	KEYBOARD_KEY_DOWNARROW_UP =							0x2128;
			const unsigned long	KEYBOARD_KEY_SELECT_UP =							0x2129;
			const unsigned long	KEYBOARD_KEY_EXECUTE_UP =							0x212B;
			const unsigned long	KEYBOARD_KEY_PRINTSCREEN_UP =						0x212C;
			const unsigned long	KEYBOARD_KEY_INSERT_UP =							0x212D;
			const unsigned long	KEYBOARD_KEY_DELETE_UP =							0x212E;
			const unsigned long	KEYBOARD_KEY_HELP_UP =								0x212F;
			const unsigned long	KEYBOARD_KEY_ZERO_UP =								0x2130;
			const unsigned long	KEYBOARD_KEY_ONE_UP =								0x2131;
			const unsigned long	KEYBOARD_KEY_TWO_UP =								0x2132;
			const unsigned long	KEYBOARD_KEY_THREE_UP =								0x2133;
			const unsigned long	KEYBOARD_KEY_FOUR_UP =								0x2134;
			const unsigned long	KEYBOARD_KEY_FIVE_UP =								0x2135;
			const unsigned long	KEYBOARD_KEY_SIX_UP =								0x2136;
			const unsigned long	KEYBOARD_KEY_SEVEN_UP =								0x2137;
			const unsigned long	KEYBOARD_KEY_EIGHT_UP =								0x2138;
			const unsigned long	KEYBOARD_KEY_NINE_UP =								0x2139;
			const unsigned long	KEYBOARD_KEY_A_UP =									0x2141;
			const unsigned long	KEYBOARD_KEY_B_UP =									0x2142;
			const unsigned long	KEYBOARD_KEY_C_UP =									0x2143;
			const unsigned long	KEYBOARD_KEY_D_UP =									0x2144;
			const unsigned long	KEYBOARD_KEY_E_UP =									0x2145;
			const unsigned long	KEYBOARD_KEY_F_UP =									0x2146;
			const unsigned long	KEYBOARD_KEY_G_UP =									0x2147;
			const unsigned long	KEYBOARD_KEY_H_UP =									0x2148;
			const unsigned long	KEYBOARD_KEY_I_UP =									0x2149;
			const unsigned long	KEYBOARD_KEY_J_UP =									0x214A;
			const unsigned long	KEYBOARD_KEY_K_UP =									0x214B;
			const unsigned long	KEYBOARD_KEY_L_UP =									0x214C;
			const unsigned long	KEYBOARD_KEY_M_UP =									0x214D;
			const unsigned long	KEYBOARD_KEY_N_UP =									0x214E;
			const unsigned long	KEYBOARD_KEY_O_UP =									0x214F;
			const unsigned long	KEYBOARD_KEY_P_UP =									0x2150;
			const unsigned long	KEYBOARD_KEY_Q_UP =									0x2151;
			const unsigned long	KEYBOARD_KEY_R_UP =									0x2152;
			const unsigned long	KEYBOARD_KEY_S_UP =									0x2153;
			const unsigned long	KEYBOARD_KEY_T_UP =									0x2154;
			const unsigned long	KEYBOARD_KEY_U_UP =									0x2155;
			const unsigned long	KEYBOARD_KEY_V_UP =									0x2156;
			const unsigned long	KEYBOARD_KEY_W_UP =									0x2157;
			const unsigned long	KEYBOARD_KEY_X_UP =									0x2158;
			const unsigned long	KEYBOARD_KEY_Y_UP =									0x2159;
			const unsigned long	KEYBOARD_KEY_Z_UP =									0x215A;
			const unsigned long	KEYBOARD_KEY_LEFTWINDOWS_UP =						0x215B;
			const unsigned long	KEYBOARD_KEY_RIGHTWINDOWS_UP =						0x215C;
			const unsigned long	KEYBOARD_KEY_APPLICATION_UP =						0x215D;
			const unsigned long	KEYBOARD_KEY_NUMPAD0_UP =							0x2160;
			const unsigned long	KEYBOARD_KEY_NUMPAD1_UP =							0x2161;
			const unsigned long	KEYBOARD_KEY_NUMPAD2_UP =							0x2162;
			const unsigned long	KEYBOARD_KEY_NUMPAD3_UP =							0x2163;
			const unsigned long	KEYBOARD_KEY_NUMPAD4_UP =							0x2164;
			const unsigned long	KEYBOARD_KEY_NUMPAD5_UP =							0x2165;
			const unsigned long	KEYBOARD_KEY_NUMPAD6_UP =							0x2166;
			const unsigned long	KEYBOARD_KEY_NUMPAD7_UP =							0x2167;
			const unsigned long	KEYBOARD_KEY_NUMPAD8_UP =							0x2168;
			const unsigned long	KEYBOARD_KEY_NUMPAD9_UP =							0x2169;
			const unsigned long	KEYBOARD_KEY_MULTIPLY_UP =							0x216A;
			const unsigned long	KEYBOARD_KEY_ADD_UP =								0x216B;
			const unsigned long	KEYBOARD_KEY_SEPARATOR_UP =							0x216C;
			const unsigned long	KEYBOARD_KEY_SUBTRACT_UP =							0x216D;
			const unsigned long	KEYBOARD_KEY_DECIMAL_UP =							0x216E;
			const unsigned long	KEYBOARD_KEY_DIVIDE_UP =							0x216F;
			const unsigned long	KEYBOARD_KEY_F1_UP =								0x2170;
			const unsigned long	KEYBOARD_KEY_F2_UP =								0x2171;
			const unsigned long	KEYBOARD_KEY_F3_UP =								0x2172;
			const unsigned long	KEYBOARD_KEY_F4_UP =								0x2173;
			const unsigned long	KEYBOARD_KEY_F5_UP =								0x2174;
			const unsigned long	KEYBOARD_KEY_F6_UP =								0x2175;
			const unsigned long	KEYBOARD_KEY_F7_UP =								0x2176;
			const unsigned long	KEYBOARD_KEY_F8_UP =								0x2177;
			const unsigned long	KEYBOARD_KEY_F9_UP =								0x2178;
			const unsigned long	KEYBOARD_KEY_F10_UP =								0x2179;
			const unsigned long	KEYBOARD_KEY_F11_UP =								0x217A;
			const unsigned long	KEYBOARD_KEY_F12_UP =								0x217B;
			const unsigned long	KEYBOARD_KEY_F13_UP =								0x217C;
			const unsigned long	KEYBOARD_KEY_F14_UP =								0x217D;
			const unsigned long	KEYBOARD_KEY_F15_UP =								0x217E;
			const unsigned long	KEYBOARD_KEY_F16_UP =								0x217F;
			const unsigned long	KEYBOARD_KEY_F17_UP =								0x2180;
			const unsigned long	KEYBOARD_KEY_F18_UP =								0x2181;
			const unsigned long	KEYBOARD_KEY_F19_UP =								0x2182;
			const unsigned long	KEYBOARD_KEY_F20_UP =								0x2183;
			const unsigned long	KEYBOARD_KEY_F21_UP =								0x2184;
			const unsigned long	KEYBOARD_KEY_F22_UP =								0x2185;
			const unsigned long	KEYBOARD_KEY_F23_UP =								0x2186;
			const unsigned long	KEYBOARD_KEY_F24_UP =								0x2187;
			const unsigned long	KEYBOARD_KEY_NUMLOCK_UP =							0x2190;
			const unsigned long	KEYBOARD_KEY_SCROLLLOCK_UP =						0x2191;
			const unsigned long	KEYBOARD_KEY_LEFTSHIFT_UP =							0x21A0;
			const unsigned long	KEYBOARD_KEY_RIGHTSHIFT_UP =						0x21A1;
			const unsigned long	KEYBOARD_KEY_LEFTCONTROL_UP =						0x21A2;
			const unsigned long	KEYBOARD_KEY_RIGHTCONTROL_UP =						0x21A3;
			const unsigned long	KEYBOARD_KEY_LEFTALT_UP =							0x21A4;
			const unsigned long	KEYBOARD_KEY_RIGHTALT_UP =							0x21A5;
			const unsigned long	KEYBOARD_KEY_SEMICOLON_UP =							0x21BA;
			const unsigned long	KEYBOARD_KEY_PLUS_UP =								0x21BB;
			const unsigned long	KEYBOARD_KEY_COMMA_UP =								0x21BC;
			const unsigned long	KEYBOARD_KEY_MINUS_UP =								0x21BD;
			const unsigned long	KEYBOARD_KEY_DOT_UP =								0x21BE;
			const unsigned long	KEYBOARD_KEY_FORWARDSLASH_UP =						0x21BF;
			const unsigned long	KEYBOARD_KEY_TILDE_UP =								0x21C0;
			const unsigned long	KEYBOARD_KEY_LEFTBRACKET_UP =						0x21DB;
			const unsigned long	KEYBOARD_KEY_BACKWARDSSLASH_UP =					0x21DC;
			const unsigned long	KEYBOARD_KEY_RIGHTBRACKET_UP =						0x21DD;
			const unsigned long	KEYBOARD_KEY_DOUBLEQUOTES_UP =						0x21DE;
			const unsigned long	KEYBOARD_KEY_PACKET_UP =							0x21E7;
			const unsigned long	KEYBOARD_KEY_ATTN_UP =								0x21F6;
			const unsigned long	KEYBOARD_KEY_CRSEL_UP =								0x21F7;
			const unsigned long	KEYBOARD_KEY_EXSEL_UP =								0x21F8;
			const unsigned long	KEYBOARD_KEY_EREOF_UP =								0x21F9;
			const unsigned long	KEYBOARD_KEY_PLAY_UP =								0x21FA;
			const unsigned long	KEYBOARD_KEY_ZOOM_UP =								0x21FB;
			const unsigned long	KEYBOARD_KEY_NONAME_UP =							0x21FC;
			const unsigned long	KEYBOARD_KEY_PA1_UP =								0x21FD;
			const unsigned long	KEYBOARD_KEY_OEMCLEAR_UP =							0x21FE;
			const unsigned long	MOUSE_POSITION =									0x2200;
			const unsigned long	MOUSE_POSITION_CHANGED =							0x2201;
			const unsigned long	MOUSE_BUTTON_LEFT_DOWN =							0x2202;
			const unsigned long	MOUSE_BUTTON_RIGHT_DOWN =							0x2203;
			const unsigned long	MOUSE_BUTTON_MIDDLE_DOWN =							0x2204;
			const unsigned long	MOUSE_BUTTON_X1_DOWN =								0x2205;
			const unsigned long	MOUSE_BUTTON_X2_DOWN =								0x2206;
			const unsigned long	MOUSE_BUTTON_LEFT_UP =								0x2207;
			const unsigned long	MOUSE_BUTTON_RIGHT_UP =								0x2208;
			const unsigned long	MOUSE_BUTTON_MIDDLE_UP =							0x2209;
			const unsigned long	MOUSE_BUTTON_X1_UP =								0x220A;
			const unsigned long	MOUSE_BUTTON_X2_UP =								0x220B;

			const unsigned long	XBOX360_CONTROLLER_LEFTSTICK_POSITION =				0x2500;
			const unsigned long	XBOX360_CONTROLLER_RIGHTSTICK_POSITION =			0x2501;
			const unsigned long	XBOX360_CONTROLLER_LEFTSTICK_POSITION_CHANGED =		0x2502;
			const unsigned long	XBOX360_CONTROLLER_RIGHTSTICK_POSITION_CHANGED =	0x2503;
			const unsigned long	XBOX360_CONTROLLER_DPAD_UP_DOWN =					0x2504;
			const unsigned long	XBOX360_CONTROLLER_DPAD_DOWN_DOWN =					0x2505;
			const unsigned long	XBOX360_CONTROLLER_DPAD_LEFT_DOWN =					0x2506;
			const unsigned long	XBOX360_CONTROLLER_DPAD_RIGHT_DOWN =				0x2507;
			const unsigned long	XBOX360_CONTROLLER_START_DOWN =						0x2508;
			const unsigned long	XBOX360_CONTROLLER_BACK_DOWN =						0x2509;
			const unsigned long	XBOX360_CONTROLLER_LEFTSTICK_DOWN =					0x250A;
			const unsigned long	XBOX360_CONTROLLER_RIGHTSTICK_DOWN =				0x250B;
			const unsigned long	XBOX360_CONTROLLER_LEFTSHOULDER_DOWN =				0x250C;
			const unsigned long	XBOX360_CONTROLLER_RIGHTSHOULDER_DOWN =				0x250D;
			const unsigned long	XBOX360_CONTROLLER_A_DOWN =							0x250E;
			const unsigned long	XBOX360_CONTROLLER_B_DOWN =							0x250F;
			const unsigned long	XBOX360_CONTROLLER_X_DOWN =							0x2510;
			const unsigned long	XBOX360_CONTROLLER_Y_DOWN =							0x2511;
			const unsigned long	XBOX360_CONTROLLER_LEFTTRIGGER_DOWN =				0x2512;
			const unsigned long	XBOX360_CONTROLLER_RIGHTTRIGGER_DOWN =				0x2513;
			const unsigned long	XBOX360_CONTROLLER_DPAD_UP_UP =						0x2524;
			const unsigned long	XBOX360_CONTROLLER_DPAD_DOWN_UP =					0x2525;
			const unsigned long	XBOX360_CONTROLLER_DPAD_LEFT_UP =					0x2526;
			const unsigned long	XBOX360_CONTROLLER_DPAD_RIGHT_UP =					0x2527;
			const unsigned long	XBOX360_CONTROLLER_START_UP =						0x2528;
			const unsigned long	XBOX360_CONTROLLER_BACK_UP =						0x2529;
			const unsigned long	XBOX360_CONTROLLER_LEFTSTICK_UP =					0x252A;
			const unsigned long	XBOX360_CONTROLLER_RIGHTSTICK_UP =					0x252B;
			const unsigned long	XBOX360_CONTROLLER_LEFTSHOULDER_UP =				0x252C;
			const unsigned long	XBOX360_CONTROLLER_RIGHTSHOULDER_UP =				0x252D;
			const unsigned long	XBOX360_CONTROLLER_A_UP =							0x252E;
			const unsigned long	XBOX360_CONTROLLER_B_UP =							0x252F;
			const unsigned long	XBOX360_CONTROLLER_X_UP =							0x2530;
			const unsigned long	XBOX360_CONTROLLER_Y_UP =							0x2531;
			const unsigned long	XBOX360_CONTROLLER_LEFTTRIGGER_UP =					0x2532;
			const unsigned long	XBOX360_CONTROLLER_RIGHTTRIGGER_UP =				0x2533;

			const unsigned long	AUDIO_MANAGER_CHANGE_UPDATE_RATE =					0x3000;

			const unsigned long	UIMANAGER_CHANGE_UPDATE_RATE =						0x5000;
			const unsigned long	RELOAD_UI =											0x5001;
			const unsigned long	LOAD_SCRIPT =										0x5002;
			const unsigned long	RUN_SCRIPT =										0x5003;
			const unsigned long	LOAD_AND_RUN_SCRIPT =								0x5004;
			const unsigned long	ADD_NULL_VARIABLE =									0x5005;
			const unsigned long	ADD_BOOL_VARIABLE =									0x5006;
			const unsigned long	ADD_INT_VARIABLE =									0x5007;
			const unsigned long	ADD_UINT_VARIABLE =									0x5008;
			const unsigned long	ADD_DOUBLE_VARIABLE =								0x5009;
			const unsigned long	ADD_STRING_VARIABLE =								0x500A;
			const unsigned long	ADD_EVENT_VARIABLE =								0x500B;
			const unsigned long	ADD_FUNCTION_VARIABLE =								0x500C;
			const unsigned long	REMOVE_NULL_VARIABLE =								0x500D;
			const unsigned long	REMOVE_BOOL_VARIABLE =								0x500E;
			const unsigned long	REMOVE_INT_VARIABLE =								0x500F;
			const unsigned long	REMOVE_UINT_VARIABLE =								0x5010;
			const unsigned long	REMOVE_DOUBLE_VARIABLE =							0x5011;
			const unsigned long	REMOVE_STRING_VARIABLE =							0x5012;
			const unsigned long	REMOVE_EVENT_VARIABLE =								0x5013;
			const unsigned long	REMOVE_FUNCTION_VARIABLE =							0x5014;
			const unsigned long	COMPONENT_ON_MOUSE_HOVER =							0x5020;
			const unsigned long	COMPONENT_ON_MOUSE_OUT =							0x5021;
			const unsigned long	COMPONENT_ON_FOCUS_GAINED =							0x5022;
			const unsigned long	COMPONENT_ON_FOCUS_LOST =							0x5023;
			const unsigned long	COMPONENT_ON_VALUE_CHANGED =						0x5024;
			const unsigned long	COMPONENT_ACTION_PERFORMED =						0x5025;
			const unsigned long	COMPONENT_ACTION_LEFT =								0x5026;	//the ACTION_LEFT and ACTION_RIGHT are intended for slider and textbox to indicate to move the slider (or caret respectively) left or right  
			const unsigned long	COMPONENT_ACTION_RIGHT =							0x5027;
			const unsigned long	COMPONENT_NAVIGATE_TO_NEXT =						0x5028;
			const unsigned long	COMPONENT_NAVIGATE_TO_PREVIOUS =					0x5029;
			const unsigned long	COMPONENT_REQUEST_FOCUS =							0x502a;	//note that this is not a "grab focus" kind of event but merely a kind request. the component id is expected at index 0 of the real data (becaues lua doesn't have access to the integral part)									
			const unsigned long	COMPONENT_REQUEST_FOCUS_PREVIOUS =					0x502b;	//note that this is not a "grab focus" kind of event but merely a kind request. the component id is expected at index 0 of the real data (becaues lua doesn't have access to the integral part)									
			const unsigned long UIMANAGER_MOUSE_MOVED =								0x502c;

			const unsigned long	SHADER_MANAGER_CHANGE_UPDATE_RATE =					0x7000;

			const unsigned long LOG_MANAGER_ENTRY_LOGGED =							0x9000;
			const unsigned long LOG_MANAGER_MESSAGE_LOGGED =						0x9001;
			const unsigned long LOG_MANAGER_WARNING_LOGGED =						0x9002;
			const unsigned long LOG_MANAGER_ERROR_LOGGED =							0x9003;




			void	setup();
			void	trigger_event( const unsigned long event_code , const Message& parameters );
			void	trigger_object_event( const unsigned long event_code , const Message& parameters );
			void	global_register_event( const unsigned long event_code );
			void	global_unregister_event( const unsigned long event_code );
			void	add_event_watch( const unsigned long event_code , Object* entity = NULL );
			void	remove_event_watch( const unsigned long event_code , Object* entity = NULL );

		}	/* Core */
	}	/* TEXEngine */



#endif		/* TEXENGINE_TEX_ENGINE_INTERFACE_HPP_INCLUDED */