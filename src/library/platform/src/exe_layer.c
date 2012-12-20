/*
 * exe_layer.c
 *
 * Created on: Dec 10, 2012
 * Author: igor
 */
#include "exe_layer.h"

uint8_t execute_agent(agent_t *agent, uint8_t opcode_size) {

	uint8_t opcodes_done = 0;

	while (opcodes_done < opcode_size) {
		//1. fetch next opcode
		uint16_t opcode = agent->code[agent->pc];

		//2. decode opcode
		opcode_t dec_opcode = decode_opcode(opcode);

		//3. execute opcode
		execute_opcode(agent, dec_opcode);

		//4. increase program counter
		if (agent->status == ready) {
			if (agent->pc < agent->code_len - 1) {
				agent->pc += 1;
			} else {
				agent->status = stopped;
				break;
			}
			opcodes_done += 1;
		} else {
			return opcodes_done;
		}

	}

	return opcodes_done;
}

opcode_t decode_opcode(uint16_t opcode) {

	opcode_t result;
	uint8_t temp;

	uint8_t nibble1 = NIBBLE1(opcode);
	uint8_t nibble2 = NIBBLE2(opcode);

	switch (nibble1) {
	//0000
	case 0:
		switch (nibble2) {

		//add reg_d, reg_r
		//0000 0011 dddd rrrr
		case 3:
			 result.id = ADD_R;
			 result.reg1 = NIBBLE3(opcode);
			 result.reg2 = NIBBLE4(opcode);
			break;

			//wait delay_ms
			//0000 0101 dddd dddd
		case 5:
			result.id = WAIT;
			result.value = BYTE2(opcode);
			break;

			//sub reg_m, reg_s
			//0000 0110 mmmm ssss
		case 6:
			result.id = SUB_R;
			result.reg1 = NIBBLE3(opcode);
			result.reg2 = NIBBLE4(opcode);
			break;

			//getservice
			//0000 0111 dddd dddd
		case 7:
			result.id = GETSERVICE;
			result.value = BYTE2(opcode);
			break;

			//priority value
			//0000 1000 pppp pppp
		case 8:
			result.id = PRIO;
			result.value = BYTE2(opcode);
			break;

			//div reg_d, reg_r
			//0000 1001 dddd rrrr
		case 9:
			result.id = DIV_R;
			result.reg1 = NIBBLE3(opcode);
			result.reg2 = NIBBLE4(opcode);
			break;

			//compare reg_d, reg_r
			//0000 1010 dddd rrrr
		case 10:
			result.id = CMP_R;
			result.reg1 = NIBBLE3(opcode);
			result.reg2 = NIBBLE4(opcode);
			break;

			//mul reg_d, reg_r
			//0000 1100 dddd rrrr
		case 12:
			result.id = MUL_R;
			result.reg1 = NIBBLE3(opcode);
			result.reg2 = NIBBLE4(opcode);
			break;

			//mv reg_d, reg_r
			//0000 1101 dddd rrrr
		case 13:
			result.id = MV;
			result.reg1 = NIBBLE3(opcode);
			result.reg2 = NIBBLE4(opcode);
			break;

		}
		break;
		//1111
	case 15:
		switch (nibble2) {
		//move service
		//1111 0001 ssss ssss
		case 1:
			result.id = MOVE;
			result.value = BYTE2(opcode);
			break;

			//clone
			//1111 0010 0000 0000
		case 2:
			result.id = CLONE;
			break;

			//jmpgr offset
			//1111 0011 vvvv vvvv
		case 3:
			result.id = JMP_G;
			result.value = get_signed_value(BYTE2(opcode));
			break;

			//die
			//1111 0100 0000 0000
		case 4:
			result.id = DIE;
			break;

			//jmpeq offset
			//1111 0110 vvvv vvvv
		case 6:
			result.id = JMP_E;
			result.value = get_signed_value(BYTE2(opcode));
			break;

			//sendmsg reg_d, agent, platform
			//1111 1000 rrrr aa pp
		case 8:
			result.id = SEND;
			result.reg1 = NIBBLE3(opcode);
			result.agent_id = HNIBBLE4(opcode);
			result.node_id = LNIBBLE4(opcode);
			break;

			//pullmsg
			//1111 1010 0000 rrrr
		case 10:
			result.id = RECV;
			result.reg1 = NIBBLE4(opcode);
			break;

			//jmpls offset
			//1111 1100 vvvv vvvv
		case 12:
			result.id = JMP_L;
			result.value = get_signed_value(BYTE2(opcode));
			break;

		}
		break;

		//add reg_r, value
		//0011 rrrr vvvv vvvv
	case 3:
		result.id = ADD_V;
		result.reg1 = NIBBLE2(opcode);
		result.value = get_signed_value(BYTE2(opcode));
		break;

		//ldh reg_d, value
		//0100 dddd vvvv vvvv
	case 4:
		result.id = STORE_L;
		result.reg1 = NIBBLE2(opcode);
		result.value = get_signed_value(BYTE2(opcode));
		break;


		//sub reg_m, value
		//0110 mmmm vvvv vvvv
	case 6:
		result.id = SUB_V;
		result.reg1 = NIBBLE2(opcode);
		result.value = get_signed_value(BYTE2(opcode));
		break;

		//setservice service, reg
		//0111 rrrr ssss ssss
	case 7:
		result.id = SETSERVICE;
		result.reg1 = NIBBLE2(opcode);
		result.value = get_signed_value(BYTE2(opcode));
		break;

		//div reg_d, value
		//1001 dddd vvvv vvvv
	case 9:
		result.id = DIV_V;
		result.reg1 = NIBBLE2(opcode);
		result.value = get_signed_value(BYTE2(opcode));
		break;

		//compare reg_d, value
		//1010 rrrr vvvv vvvv
	case 10:
		result.id=CMP_V;
		result.reg1=NIBBLE2(opcode);
		result.value = get_signed_value(BYTE2(opcode));
		break;

		//storecr reg_str, char
		//1011 rrrr vvvv vvvv
	case 11:
		result.id = STORE_C;
		result.reg1 = NIBBLE2(opcode);
		result.value = BYTE2(opcode);
		break;

		//mul reg1, value
		//1100 dddd vvvv vvvv
	case 12:
		result.id = MUL_V;
		result.reg1 = NIBBLE2(opcode);
		result.value = get_signed_value(BYTE2(opcode));
		break;

		//ldh reg_d, value
		//1101 dddd vvvv vvvv
	case 13:
		result.id = STORE_H;
		result.reg1 = NIBBLE2(opcode);
		result.value = BYTE2(opcode);
		break;

	default:
		break;
	}

	return result;
}

void execute_opcode(agent_t *agent, opcode_t opcode) {
	uint16_t tmp = 0;
	int16_t sgn_tmp = 0;

	switch (opcode.id) {
	case SETSERVICE:
		PRINTF("setservice service_id: %d, reg: %d\n", opcode.value, opcode.reg1);

		switch (opcode.value) {

		case SERVICE_BARGRAPH:
			if (platform.drivers.set_bargraph != NULL) {
				platform.drivers.set_bargraph((agent->regs[opcode.reg1] & 0x00ff));
			} else {
				SET_ERROR(agent->status_flag, ERROR_NO_SERVICE_PRESENT);
			}
			break;

		default:
			break;

		}
		break;
		case GETSERVICE:
			printf("getservice service_id: %d\n", opcode.value);
			switch (opcode.value){
			case SERVICE_THERMOMETER:
				if (platform.drivers.therm_get_temp != NULL){

					tmp = (platform.drivers.therm_get_temp(THERMOMETER1) >>3);
					tmp += (platform.drivers.therm_get_temp(THERMOMETER2) >>3);
					tmp += (platform.drivers.therm_get_temp(THERMOMETER3) >>3);
					tmp /= 3;
					agent->regs[REG_ACC] = tmp;


				} else {
					SET_ERROR(agent->status_flag, ERROR_NO_SERVICE_PRESENT);
				}
				break;
			default:
				break;
			}

			break;

	case STORE_L:
		PRINTF ("ldl reg_d:%d, value:%d\n", opcode.reg1, opcode.value);
		agent->regs[opcode.reg1] = opcode.value;
		break;

	case ADD_R:
		PRINTF("add reg_d: %d , reg_r: %d\n", opcode.reg1, opcode.reg2);
		tmp = (~(agent->regs[opcode.reg1] ^ agent->regs[opcode.reg2] ) & 0x8000);
		agent->regs[REG_ACC] = agent->regs[opcode.reg1] + agent->regs[opcode.reg2];
		if (tmp != 0){
			if (opcode.reg1 != REG_ACC){
				agent->status_flag = (((agent->regs[REG_ACC] ^ agent->regs[opcode.reg1]) & 0x8000) << 24);
			} else {
				agent->status_flag = (((agent->regs[REG_ACC] ^ agent->regs[opcode.reg2]) & 0x8000) << 24);
			}
		} else {
			agent->status_flag &= ~OVERFLOW_MASK;
		}
		break;

	case ADD_V:
		PRINTF("add reg_r:%d, value:%d\n", opcode.reg1, opcode.value);
		agent->regs[REG_ACC] = agent->regs[opcode.reg1] + opcode.value;
		break;

	case SUB_R:
		PRINTF("sub reg_m:%d, reg_s:%d\n", opcode.reg1, opcode.reg2);
		agent->regs[REG_ACC] = agent->regs[opcode.reg1] - agent->regs[opcode.reg2];
		break;

	case SUB_V:
		PRINTF("sub reg_m:%d, value:%d\n", opcode.reg1, opcode.value);
		agent->regs[REG_ACC] = agent->regs[opcode.reg1] - opcode.value;
		break;

	case DIV_R:
		PRINTF("div reg_d:%d, reg_r:%d\n", opcode.reg1, opcode.reg2);
		agent->regs[REG_ACC] = agent->regs[opcode.reg1] / agent->regs[opcode.reg2];
		break;

	case DIV_V:
		PRINTF("div reg_d:%d, value:%d\n", opcode.reg1, opcode.value);
		agent->regs[REG_ACC] = agent->regs[opcode.reg1] / opcode.value;
		break;

	case MUL_R:
		PRINTF("mul reg_d:%d, reg_r:%d\n", opcode.reg1, opcode.reg2);
		agent->regs[REG_ACC] = agent->regs[opcode.reg1] * agent->regs[opcode.reg2];
		break;

	case MUL_V:
		PRINTF("mul reg1:%d, value:%d\n", opcode.reg1, opcode.value);
		agent->regs[REG_ACC] = agent->regs[opcode.reg1] * opcode.value;
		break;

	case JMP_G:
		PRINTF("jmpgr offset:%d\n", opcode.value);
		if (agent->regs[REG_ACC]==1) {
			agent->pc = agent->pc + opcode.value;
		}
		break;

	case JMP_E:
		PRINTF("jmpeq offset:%d\n", opcode.value);
		if (agent->regs[REG_ACC]==0){
			agent->pc = agent->pc + opcode.value;
		}
		break;

	case JMP_L:
		PRINTF("jmpls offset:%d\n", opcode.value);
		if (agent->regs[REG_ACC]==-1){
			agent->pc = agent->pc + opcode.value;
		}
		break;

	case CMP_R:
		PRINTF("compare reg_d:%d, reg_r:%d\n", opcode.reg1, opcode.reg2);

		if (agent->regs[opcode.reg1] > agent->regs[opcode.reg2]){
			agent->regs[REG_ACC] = 1;
		} else if (agent->regs[opcode.reg1] == agent->regs[opcode.reg2]){
			agent->regs[REG_ACC] = 0;
		} else if (agent->regs[opcode.reg1] < agent->regs[opcode.reg2]){
			agent->regs[REG_ACC] = -1;
		}
		break;

	case CMP_V:
		PRINTF("compare reg_d:%d, value:%d\n", opcode.reg1, opcode.value);

		if (agent->regs[opcode.reg1] > opcode.value){
			agent->regs[REG_ACC] = 1;
		} else if (agent->regs[opcode.reg1] == opcode.value){
			agent->regs[REG_ACC] = 0;
		} else if (agent->regs[opcode.reg1] < opcode.value){
			agent->regs[REG_ACC] = -1;
		}
		break;

	case MOVE:
		PRINTF("move service:%d\n", opcode.value);
		//TODO
		break;

	case CLONE:
		PRINTF("clone\n");
		agent->regs[REG_ACC] = clone_agent(agent);
		break;

	case DIE:
		PRINTF("die\n");
		agent->status = stopped;
		break;

	case SEND:
		PRINTF("sendmsg reg:%d, agent:%d, platform:%d\n", opcode.reg1, opcode.agent_id, opcode.node_id);
		//TODO
		break;

	case RECV:
		PRINTF("pullmsg reg:%d\n", opcode.reg1);
		//TODO
		break;

	case STORE_H:
		PRINTF("ldh reg_d:%d, value:%d\n", opcode.reg1, opcode.value);
		sgn_tmp = agent->regs[opcode.reg1];
		tmp =  (opcode.value << 8);
		sgn_tmp = (sgn_tmp & 0x00FF) | tmp;
		agent->regs[opcode.reg1] = sgn_tmp;
		break;

	case STORE_C:
		PRINTF("storecr reg_str:%d, char:%d\n", opcode.reg1, opcode.value);
		agent->reg_str[opcode.reg1] = (char*) realloc (agent->reg_str[opcode.reg1], agent->regstr_len[opcode.reg1] + 1);
		agent->reg_str[opcode.reg1][agent->regstr_len[opcode.reg1]] = opcode.value;
		agent->regstr_len[opcode.reg1]+= 1;
		break;

	case MV:
		PRINTF("mv reg_d:%d, reg_r:%d\n", opcode.reg1, opcode.reg2);
		agent->regs[opcode.reg1] = agent->regs[opcode.reg2];
		break;

	case WAIT:
		PRINTF("wait delay_ms:%d\n", opcode.value);
		//TODO
		break;

	case PRIO:
		PRINTF("priority value:%d\n", opcode.value);
		agent->priority = opcode.value;
		break;


	default:
		break;

	}

}

int16_t get_signed_value(uint8_t value) {
	int16_t result = 0;

	if ((value & BYTE_SIGN) != 0) {
		result = value | NEG_SIGN;
	} else {
		result = value & POS_SIGN;
	}
	return result;
}