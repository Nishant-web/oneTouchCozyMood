/*
 * Library : R305 Finger Print Sensor
 * File    : fpsR305.h [header]
 */

#ifndef FPSR305_H__
#define FPSR305_H__

class FPS_R305
{
	private:
		// My Property
		unsigned int fpsr305_serial_port_br;
		int fpsr305_serial_port_id;
		const char *fpsr305_serial_port_name;
	public:
		// Constructor Declarations
		FPS_R305(void);
		FPS_R305(const char*, const unsigned int);

		// Member Function Declarations
		bool init(void);
		void set_password(uint32_t);
		int8_t get_resp(uint8_t*, uint8_t);
		bool GenImg(void);
		bool Img2Tz(uint8_t);
		bool RegModel(void);
		bool Store(uint16_t);
		bool enroll_id(uint16_t);
		int16_t scan_id(void);
		bool delete_id(uint16_t);
		bool delete_all(void);
		bool get_bytes(uint8_t*, uint16_t);
		void send_bytes(uint8_t*, uint16_t);
		uint16_t get_sum(uint8_t*, uint16_t, uint16_t);
};

#endif //FPSR305_H__
