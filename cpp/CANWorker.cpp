/*
 * CANWorker.cpp
 *
 *  Created on: 27 янв. 2019 г.
 *      Author: yura
 */

#ifdef USE_HAL_DRIVER
#include <CANWorker.h>

namespace canw {

//####################################################################################//
//################################ - Initialisation - ################################//
//####################################################################################//
CAN_Worker::CAN_Worker(CAN_HandleTypeDef *hcan, uint32_t std_id) {
	this->hcan = hcan;
	this->set_CANTX_id_type(can_id_type::STD);
	this->set_CANRX_id_type(can_id_type::STD);
	this->set_CANRX_PACK_SIZE((can_pack_size)PACK_SIZE_DEFAULT);
	this->set_CANTX_PACK_SIZE((can_pack_size)PACK_SIZE_DEFAULT);
	this->set_id(std_id);
	mailBox = HAL_CAN_GetTxMailboxesFreeLevel(hcan);
	init();
}

CAN_Worker::CAN_Worker(CAN_HandleTypeDef *hcan, uint32_t id, can_id_type type){
	this->hcan = hcan;
	mailBox = HAL_CAN_GetTxMailboxesFreeLevel(hcan);
	this->set_CANTX_id_type(type);
	this->set_CANRX_id_type(type);
	this->set_CANRX_PACK_SIZE((can_pack_size)PACK_SIZE_DEFAULT);
	this->set_CANTX_PACK_SIZE((can_pack_size)PACK_SIZE_DEFAULT);
	this->set_id(id);
	init();
}

CAN_Worker::CAN_Worker(CAN_HandleTypeDef *hcan, uint32_t std_id, can_pack_size size){
	this->hcan = hcan;
	this->set_CANTX_id_type(can_id_type::STD);
	this->set_CANRX_id_type(can_id_type::STD);
	this->set_CANRX_PACK_SIZE(size);
	this->set_CANTX_PACK_SIZE(size);
	this->set_id(std_id);
	mailBox = HAL_CAN_GetTxMailboxesFreeLevel(hcan);
	init();
}

CAN_Worker::CAN_Worker(CAN_HandleTypeDef *hcan, uint32_t id, can_pack_size size, can_id_type type){
	this->hcan = hcan;
	mailBox = HAL_CAN_GetTxMailboxesFreeLevel(hcan);
	this->set_CANTX_id_type(type);
	this->set_CANRX_id_type(type);
	this->set_CANRX_PACK_SIZE(size);
	this->set_CANTX_PACK_SIZE(size);
	this->set_id(id);
	init();
}

CAN_Worker::~CAN_Worker() {
	// TODO Auto-generated destructor stub
	CAN_filter = {0};

	HAL_CAN_ConfigFilter(this->hcan, &CAN_filter);
	HAL_CAN_Stop(this->hcan);
	disable_itterupt();
}

bool CAN_Worker::init(){
	// setting header TX message
//	CAN_TXHeader.DLC					= CAN_PACK_SIZE;
//	CAN_TXHeader.StdId 					= CAN_TXHeader.StdId;
//	CAN_TXHeader.ExtId 					= 0;
//	CAN_TXHeader.IDE					= CAN_ID_STD;
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
//	CAN_RXHeader.DLC 					= CAN_PACK_SIZE;
//	CAN_RXHeader.StdId 					= CAN_TXHeader.StdId;
//	CAN_RXHeader.ExtId 					= 0;
//	CAN_RXHeader.IDE					= CAN_ID_STD;
	CAN_RXHeader.RTR 					= CAN_RTR_DATA;

	if (HAL_CAN_ConfigFilter(hcan, &CAN_filter) != HAL_OK)
	{
		/* Filter configuration Error */
		Error_Handler();
		return false;
	}

	if (HAL_CAN_Start(hcan) != HAL_OK)
	{
		/* Start Error */
		Error_Handler();
		return false;
	}

	if (!enable_itterupt()){
		return false;
	}

	return true;
}

void CAN_Worker::set_id(uint32_t id){
	if (CAN_TXHeader.IDE == CAN_ID_EXT){
		CAN_TXHeader.ExtId = id;
	} else {
		CAN_TXHeader.StdId = id;
	}
}

void CAN_Worker::set_CANTX_id_type(can_id_type type){
	CAN_TXHeader.IDE = (uint32_t)type;
}

void CAN_Worker::set_CANRX_id_type(can_id_type type){
	CAN_RXHeader.IDE = (uint32_t)type;
}

void CAN_Worker::set_CANTX_PACK_SIZE(can_pack_size size){
	CAN_TXHeader.DLC = (uint8_t)size;
}

void CAN_Worker::set_CANRX_PACK_SIZE(can_pack_size size){
	CAN_RXHeader.DLC = (uint8_t)size;
}

void CAN_Worker::set_RTR_TX(can_rtr_type type){
	CAN_TXHeader.RTR = (uint32_t)type;
}

void CAN_Worker::set_RTR_RX(can_rtr_type type){
	CAN_RXHeader.RTR = (uint32_t)type;
}

bool CAN_Worker::set_filter(CAN_FilterTypeDef *filter){
	if (HAL_CAN_ConfigFilter(hcan, filter) != HAL_OK)
	{
		/* Filter configuration Error */
		Error_Handler();
		return false;
	}
	return true;
}

bool CAN_Worker::set_filter_Bank(uint8_t filter_Bank){
	CAN_filter.FilterBank = filter_Bank;

}

bool CAN_Worker::enable_itterupt(){
	if (HAL_CAN_ActivateNotification(this->hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK){
		Error_Handler();
		return false;
	}
	if (HAL_CAN_ActivateNotification(this->hcan, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK){
		Error_Handler();
		return false;
	}
	if (HAL_CAN_ActivateNotification(this->hcan, CAN_IT_ERROR) != HAL_OK){
		Error_Handler();
		return false;
	}

	return true;
}

bool CAN_Worker::disable_itterupt(){
	if (HAL_CAN_DeactivateNotification(this->hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK){
		Error_Handler();
		return false;
	}
	if (HAL_CAN_DeactivateNotification(this->hcan, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK){
		Error_Handler();
		return false;
	}
	if (HAL_CAN_DeactivateNotification(this->hcan, CAN_IT_ERROR) != HAL_OK){
		Error_Handler();
		return false;
	}
	return true;
}

//####################################################################################//
//################################## - Data's send - #################################//
//####################################################################################//
bool CAN_Worker::send_data(uint8_t *data){
	if (HAL_CAN_AddTxMessage(hcan, &CAN_TXHeader, data, &mailBox) != HAL_OK){
		return false;
		Error_Handler();
	}
	return true;
}

bool CAN_Worker::send_data(uint8_t *data, uint32_t id){
	this->set_id(id);
	return this->send_data(data);
}

//####################################################################################//
//################################## - Callback's - ##################################//
//####################################################################################//
void CAN_Worker::CAN_RX0_Callback(){
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &CAN_RXHeader, rx_fifo0) != HAL_OK){
		Error_Handler();
	}
}

void CAN_Worker::CAN_RX1_Callback(){
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &CAN_RXHeader, rx_fifo1) != HAL_OK){
		Error_Handler();
	}
}

void CAN_Worker::CAN_TX_Callback(){

}

void CAN_Worker::CAN_ERR_Callback(){
	// todo - Дописать анализ ошибок
	uint32_t err_code = HAL_CAN_GetError(hcan);
	if (err_code == CAN_IT_ERROR_WARNING){  	/*!< Error warning interrupt          */

	}
	if (err_code == CAN_IT_ERROR_PASSIVE){ 		/*!< Error passive interrupt          */

	}
	if (err_code == CAN_IT_BUSOFF){				/*!< Bus-off interrupt                */

	}
	if (err_code == CAN_IT_LAST_ERROR_CODE){	/*!< Last error code interrupt        */

	}
	if (err_code == CAN_IT_ERROR){				/*!< Error Interrupt                  */

	}
}

} /* namespace canw */

#endif // USE_HAL_DRIVER
