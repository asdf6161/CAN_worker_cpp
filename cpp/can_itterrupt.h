/*
 * can_itterrupt.h
 *
 *  Created on: 27 џэт. 2019 у.
 *      Author: yura
 */

#ifndef CAN_ITTERRUPT_H_
#define CAN_ITTERRUPT_H_

#ifdef __cplusplus
extern "C" {
#endif

// Callback'ss
void CAN_RX0_Callback();
void CAN_RX1_Callback();
void CAN_TX_Callback();
void CAN_ERR_Callback();

#ifdef __cplusplus
};
#endif

#endif /* CAN_ITTERRUPT_H_ */
