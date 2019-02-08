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

#define CAN_PACK_SIZE 3
#define CAN_ID 3

class CAN_Worker {
public:
	CAN_Worker(CAN_HandleTypeDef *hcan);
	virtual ~CAN_Worker();

// Methods
public:
	bool init();
	bool send_data(uint8_t *data);

	// Callback's
	void CAN_RX0_Callback();
	void CAN_RX1_Callback();
	void CAN_TX_Callback();
	void CAN_ERR_Callback();

// PV methods
public:
	// Enable itterrupt
	bool enable_itterupt();

// Vars
public:
	CAN_HandleTypeDef 		*hcan;
	CAN_TxHeaderTypeDef 	CAN_TXHeader;
	CAN_FilterTypeDef 		CAN_filter;
	CAN_RxHeaderTypeDef 	CAN_RXHeader;
	uint32_t			 	mailBox;
	uint8_t 				rx_fifo[CAN_PACK_SIZE];
};

} /* namespace canw */

#endif /* CANWORKER_H_ */

#endif // USE_HAL_DRIVER
