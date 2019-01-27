/*
 * can_itterrupt.cpp
 *
 *  Created on: 27 џэт. 2019 у.
 *      Author: yura
 */

#include "can_itterrupt.h"
#include "CANWorker.h"

extern canw::CAN_Worker *can_worker_ptr;

void CAN_RX0_Callback(){
	can_worker_ptr->CAN_RX0_Callback();
}

void CAN_RX1_Callback(){
	can_worker_ptr->CAN_RX1_Callback();
}

void CAN_TX_Callback(){
	can_worker_ptr->CAN_TX_Callback();
}

void CAN_ERR_Callback(){
	can_worker_ptr->CAN_ERR_Callback();
}

