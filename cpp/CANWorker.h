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

class CAN_Worker {
public:
	CAN_Worker(CAN_HandleTypeDef *hcan);
	virtual ~CAN_Worker();

// Methods
public:
	bool init();

	// Callback's
	void CAN_RX0_Callback();
	void CAN_RX1_Callback();
	void CAN_TX_Callback();
	void CAN_ERR_Callback();

// PV methods
private:
	// Enable itterrupt
	bool enable_itterupt();

// Vars
private:
	CAN_HandleTypeDef 		*hcan;
	CAN_TxHeaderTypeDef 	CAN_TXHeader;
	CAN_FilterTypeDef 		CAN_filter;
	CAN_RxHeaderTypeDef 	CAN_RXHeader;
	CAN_TxMailBox_TypeDef 	mailBox;
};

} /* namespace canw */

#endif /* CANWORKER_H_ */

#endif // USE_HAL_DRIVER
