/*
 * CANWorker.h
 *
 *  Created on: 27 џэт. 2019 у.
 *      Author: yura
 */

#ifdef USE_HAL_DRIVER
#ifndef CANWORKER_H_
#define CANWORKER_H_

#include "main.h"

namespace canw {

#define PACK_SIZE_DEFAULT 3

enum class can_id_type {
	STD = (0x00000000U),
	EXT = (0x00000004U)
};

enum class can_pack_size {
	_1_BYTE = 1,
	_2_BYTE,
	_3_BYTE,
	_4_BYTE,
	_5_BYTE,
	_6_BYTE,
	_7_BYTE,
	_8_BYTE,
};

enum class can_rtr_type {
	RTR_DATA = (0x00000000U),
	RTR_REMOTE = (0x00000002U)
};

class CAN_Worker {
public:
	CAN_Worker(CAN_HandleTypeDef *hcan, uint32_t std_id);
	CAN_Worker(CAN_HandleTypeDef *hcan, uint32_t id, 	 can_id_type type);
	CAN_Worker(CAN_HandleTypeDef *hcan, uint32_t std_id, can_pack_size size);
	CAN_Worker(CAN_HandleTypeDef *hcan, uint32_t id, 	 can_pack_size size, can_id_type type);
	virtual ~CAN_Worker();

// Methods
public:
	bool init();
	bool send_data(uint8_t *data);
	bool send_data(uint8_t *data, uint32_t id);

	// getter's and setter's
	void set_id(uint32_t id);
	void set_CANTX_id_type(can_id_type type);
	void set_CANRX_id_type(can_id_type type);
	void set_CANTX_PACK_SIZE(can_pack_size size);
	void set_CANRX_PACK_SIZE(can_pack_size size);
	void set_RTR_TX(can_rtr_type type);
	void set_RTR_RX(can_rtr_type type);
	bool set_filter(CAN_FilterTypeDef *filter);
	bool set_filter_Bank(uint8_t filter_Bank);

	// Callback's
	void CAN_RX0_Callback();
	void CAN_RX1_Callback();
	void CAN_TX_Callback();
	void CAN_ERR_Callback();

// PV methods
private:
	// Enable itterrupt
	bool enable_itterupt();
	bool disable_itterupt();

// Vars
public:
	CAN_HandleTypeDef 		*hcan 			= nullptr;
	CAN_TxHeaderTypeDef 	CAN_TXHeader 	= {0};
	CAN_FilterTypeDef 		CAN_filter 		= {0};
	CAN_RxHeaderTypeDef 	CAN_RXHeader 	= {0};
	uint32_t			 	mailBox 		= 0;
	uint8_t 				rx_fifo0[8]		= {0};
	uint8_t 				rx_fifo1[8]		= {0};

};

} /* namespace canw */

#endif /* CANWORKER_H_ */

#endif // USE_HAL_DRIVER
