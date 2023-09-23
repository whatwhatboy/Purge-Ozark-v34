#include "hooks.h"
#include "../../rage/invoker/natives.h"
#include "../../util/log.h"
#include "../../util/memory/pattern.h"

static inline __int64 get_sync_tree_from_id(unsigned __int16 id)
{
	return x64::spoof_call(global::vars::GetSyncTreeFromId, &x64::fastcall_ctx, (__int64)NULL, (unsigned __int16)id);
}


__int64 menu::hooks::read_from_sync_tree_buffer(unsigned __int64** pNetSyncTree, __int64 a2, unsigned int a3, __int64 a4, __int64 a5)
{

	uint32_t mHash = 0;
	__int64 pCEntityCreationDataNode = NULL;

	for (int i = 1; i < 13; i++)
	{
		if (get_sync_tree_from_id(i) == (__int64)pNetSyncTree)
		{
			switch (i) 
			{
			case 5:  
				
				LOG("CObjectSyncTree: %p", pNetSyncTree);

				pCEntityCreationDataNode = *(__int64*)((unsigned __int64)pNetSyncTree + 48);
				mHash = *(uint32_t*)(pCEntityCreationDataNode + 336);

				if (!native::is_model_valid(mHash))
				{
					*(uint32_t*)(pCEntityCreationDataNode + 336) = joaat("prop_paints_can07");
					LOG("Invalid sync model");
				}
				break;

				switch (mHash)
				{
				case 379820688: 
				case 1692612370: 
				case 959275690: 
				case 1396140175: 

					LOG("Cage spawn");
					*(uint32_t*)(pCEntityCreationDataNode + 336) = joaat("prop_paints_can07");
				}

				break;

			case 11:  
				break;

			case 0:  
			case 1:	 
			case 2:	 
			case 4:  
			case 9:  
			case 12:
			case 13:

				pCEntityCreationDataNode = *(__int64*)((unsigned __int64)pNetSyncTree + 48);
				mHash = *(uint32_t*)(pCEntityCreationDataNode + 200);
				
				if (!native::is_model_valid(mHash))
				{
				*(uint32_t*)(pCEntityCreationDataNode + 200) = joaat("comet");
				LOG("Invalid sync model");;
				}
				break;
			}
			break;
		}
	}
	
	return read_from_sync_tree_buffer_t(pNetSyncTree, a2,a3,a4,a5);
}
