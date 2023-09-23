#include "hooks.h"
#include "../../rage/invoker/natives.h"
#include "../../util/log.h"

char __cdecl menu::hooks::script_event_protection(__int64 netGameEvent, __int64 sender)
{
	auto data = (__int64*)(netGameEvent + 112);
	Player sender_id = *(__int8*)(sender + 33);
	LPCSTR sender_name = native::get_player_name(sender_id);

//	LOG(XOR("scripted event, from: %s, hash: %d"), sender_name, data[0]);

	switch ((uint32_t)data[0])
	{
	case 1810531023:
//		LOG(XOR("Kick event, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case 1674887089:
	//	LOG(XOR("Kick event, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case -852914485:
	//	LOG(XOR("Kick from vehicle, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case -910497748:
	//	LOG(XOR("Island, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case 1327169001:
	//	LOG(XOR("Island, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case -1057685265:
//		LOG(XOR("Island, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case 392606458:
	//	LOG(XOR("Cutscene, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case 1858712297:
	//	LOG(XOR("Send to mission, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case 1250480109:
	//	LOG(XOR("Send to mission, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case 113023613:
	//	LOG(XOR("Send to mission, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case -168599209:
	//	LOG(XOR("Black Screen, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case 1775863255:
	//	LOG(XOR("Crash, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case -904555865:
	//	LOG(XOR("Crash, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;


	case 915462795:
	//	LOG(XOR("Kick, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case 1214823473:
	//	LOG(XOR("Send to location, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case 434937615:
	//	LOG(XOR("Send to warehouse, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case -964882004:
		//LOG(XOR("Destroy personal vehicle, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case -1891171016:
	//	LOG(XOR("Sound spam, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case 316066012:
	//	LOG(XOR("Ceo ban, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case -1831959078:
	//	LOG(XOR("Ceo kick vehicle, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

	case 245065909:
	//	LOG(XOR("Ceo Money, from: %s, hash: %d"), sender_name, data[0]);
		return TRUE;

		//1891171016

		/*
		* 	CeoBan = 316066012, // mpply_vipgameplaydisabledtimer
		CeoKick = -1831959078, // BGDISMISSED
		CeoMoney = 245065909, // Goon_Paid_Large
	
		Crash = -904555865, // SET_NO_LOADING_SCREEN, xref it
		Crash2 = 1775863255,
		*/

	default:
		break;
	}
	return script_event_protection_t(netGameEvent, sender);
}
