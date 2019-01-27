/*
 * CANWorker.cpp
 *
 *  Created on: 27 џэт. 2019 у.
 *      Author: yura
 */

#ifdef USE_HAL_DRIVER
#include <CANWorker.h>

namespace canw {

//####################################################################################//
//################################ - Initialisation - ################################//
//####################################################################################//
CAN_Worker::CAN_Worker(CAN_HandleTypeDef *hcan) {
	this->hcan = hcan;
	CAN_TXHeader = {0};
	CAN_filter = {0};
	CAN_RXHeader = {0};
	mailBox = {0};
	init();
}

CAN_Worker::~CAN_Worker() {
	// TODO Auto-generated destructor stub
}

bool CAN_Worker::init(){
	// setting header TX message
	CAN_TXHeader.StdId 					= 55;
	CAN_TXHeader.ExtId 					= 0;
	CAN_TXHeader.IDE					= CAN_ID_STD;
	CAN_TXHeader.RTR 					= CAN_RTR_DATA;
	CAN_TXHeader.TransmitGlobalTime 	= DISABLE;

	// setting filter
	CAN_filter.FilterBank 				= 0;
	CAN_filter.FilterMode 				= CAN_FILTERMODE_IDMASK;
	CAN_filter.FilterScale 				= CAN_FILTERSCALE_32BIT;
	CAN_filter.FilterIdHigh 			= 0x0000;
	CAN_filter.FilterIdLow 				= 0x0000;
	CAN_filter.FilterMaskIdHigh 		= 0x0000;
	CAN_filter.FilterMaskIdLow 			= 0x0000;
	CAN_filter.FilterFIFOAssignment 	= CAN_RX_FIFO0;
	CAN_filter.FilterActivation 		= ENABLE;
	CAN_filter.SlaveStartFilterBank 	= 14;

	// setting header RX message
	CAN_RXHeader.DLC 					= 3;
	CAN_RXHeader.StdId 					= 55;
	CAN_RXHeader.ExtId 					= 0;
	CAN_RXHeader.IDE					= CAN_ID_STD;
	CAN_RXHeader.RTR 					= CAN_RTR_DATA;

	if (HAL_CAN_ConfigFilter(hcan, &CAN_filter) != HAL_OK)
	{
		/* Filter configuration Error */
		return false;
	}

	if (HAL_CAN_Start(hcan) != HAL_OK)
	{
		/* Start Error */
		return false;
	}

	if (!enable_itterupt()){
		return false;
	}

	return true;
}

bool CAN_Worker::enable_itterupt(){
	if (HAL_CAN_ActivateNotification(this->hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK){
		return false;
	}
	if (HAL_CAN_ActivateNotification(this->hcan, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK){
		return false;
	}
	if (HAL_CAN_ActivateNotification(this->hcan, CAN_IT_ERROR) != HAL_OK){
		return false;
	}

	return true;
}

//####################################################################################//
//################################## - Callback's - ##################################//
//####################################################################################//
void CAN_Worker::CAN_RX0_Callback(){

}

void CAN_Worker::CAN_RX1_Callback(){

}

void CAN_Worker::CAN_TX_Callback(){

}

void CAN_Worker::CAN_ERR_Callback(){

}

} /* namespace canw */

#endif // USE_HAL_DRIVER
