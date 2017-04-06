#define VRSOFT_PLUGIN_EXPORT
#include "../../Public/VRSoft.h"
#include "AirLineManager.h"
#include "TrajectLineEntityBuilder.h"
#include "BallMissileTrajectBuilder.h"

namespace VR_Soft
{
	/////////////////////////////////////
	// ע��ص�����
	typedef void (*DLL_INSTANLL_IAIRLINE)(IEntityFactoryManager* pIEntityFactoryManager);
	typedef void (*DLL_UNINSTANLL_IAIRLINE)(IEntityFactoryManager* pIEntityFactoryManager);
	////////////////////////////////////

	// ���캯��
	CAirLineManager::CAirLineManager(IEntityFactoryManager* pIEntityFactoryManager)
		:m_pIEntityFactoryManager(pIEntityFactoryManager)
	{
	}

	// ��������
	CAirLineManager::~CAirLineManager(void)
	{
		IEntityFactoryManager* pEntityFactoryManager = IEntityFactoryManager::GetInstancePtr();
		if (NULL == pEntityFactoryManager)
		{
			return;
		}
		m_pIEntityFactoryManager->UnRegisterFactory("�߶�");
		m_pIEntityFactoryManager->UnRegisterFactory("������������");
	}

	// ���ҵ�ǰ�ļ��¼��ز��
	void CAirLineManager::LoadAllElecDll(void)
	{
		// ��ȡ��������еĲ���ļ�
		//const VRString strElecDllPath = IComManager::GetInstance().GetComManagerConfig()->GetCurPath() + "/AirLine/";

		//typedef std::vector<VRString> Files;
		//Files files;
		//CFileOperetor::GetFilesInPath(strElecDllPath, "*.dll", files);

		//// �������е��ļ�
		//Files::const_iterator cstItor = files.begin();
		//for (; files.end() != cstItor; ++cstItor)
		//{
		//	LoadElecDll(strElecDllPath + (*cstItor));
		//}

		// ����ʵ��
	//	m_pIEntityFactoryManager->RegisterFactory(CREATE_BASEENTITY_FACTORY(CTrajectLineEntityBuilder));
//		m_pIEntityFactoryManager->RegisterFactory(CREATE_BASEENTITY_FACTORY(CBallMissileTrajectBuilder));
	}

	// ��ǰ��̬���Ƿ������
	bool CAirLineManager::IsLoaded(const VRString& strElecName)
	{
		/*	ListEquipPlugin::const_iterator cstItor = m_lstEquipPlugin.find(strElecName);
		return (m_lstEquipPlugin.end() != cstItor);*/
		return (true);
	}

	// ���ص���dll�ļ�
	void CAirLineManager::LoadElecDll(const VRString& strElecName)
	{
		// �жϵ�ǰ�ļ��Ѿ�������
		//if (IsLoaded(strElecName))
		//{
		//	return ;
		//}

		//// �����ļ�
		//CDyLib* pDyLib = new CDyLib(strElecName);
		//if (NULL == pDyLib)
		//{
		//	IComManager::GetInstance().WriteLogMsg("���غ��߲��:"+ strElecName +" ʧ��", ERROR_MALLOC);
		//	return;
		//}

		//try
		//{
		//	pDyLib->Load();
		//}
		//catch (...)
		//{
		//	return;
		//}

		//// ������ں���
		//DLL_INSTANLL_IAIRLINE pFunction = (DLL_INSTANLL_IAIRLINE)pDyLib->GetSymbol("InstallEquipPlugin");

		//if (NULL == pFunction)
		//{
		//	IComManager::GetInstance().WriteLogMsg("���غ��߲��:"+ strElecName +"δʶ��", ERROR_NOT_FIND);
		//	delete pDyLib;
		//	return;
		//}

		//// ���뵽����
		//m_lstEquipPlugin[strElecName] = pDyLib;

		//// ִ�к���
		//pFunction(m_pIEntityFactoryManager);
	}

	// ж�ص���dll�ļ�
	void CAirLineManager::UnLoadElecDll(const VRString& strElecName)
	{
		m_pIEntityFactoryManager->UnRegisterFactory("�߶�");
		m_pIEntityFactoryManager->UnRegisterFactory("������������");
	/*	// �жϵ�ǰ�ļ��Ѿ�������
		if (!IsLoaded(strElecName))
		{
			return ;
		}

		ListEquipPlugin::iterator itor = m_lstEquipPlugin.find(strElecName);
		CDyLib* pDyLib = itor->second;
		if (NULL == pDyLib)
		{
			return;
		}

		// ������ں���
		DLL_UNINSTANLL_IAIRLINE pFunction = (DLL_UNINSTANLL_IAIRLINE)pDyLib->GetSymbol("UnInstallEquipPlugin");
		if (NULL == pFunction)
		{
			IComManager::GetInstance().WriteLogMsg("���غ��߲��:"+ strElecName +"δʶ��", ERROR_NOT_FIND);
			delete pDyLib;
			m_lstEquipPlugin.erase(itor);
			return;
		}

		pFunction(m_pIEntityFactoryManager);
		m_lstEquipPlugin.erase(itor);
		*/
	}
}
