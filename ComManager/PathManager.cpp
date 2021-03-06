#include "../../Public/VRSoft.h"
#include "PathManager.h"
#include "VRPath.h"

#include <algorithm>

namespace VR_Soft
{
	CPathManager::CPathManager(void)
	{
	}


	CPathManager::~CPathManager(void)
	{
		RemoveAll();
	}

	// 添加路径
	IPath* CPathManager::Create(const uint64_t uID, const VRString& strName, IFeatureGeomtry* pIFeatureGeomtry, IPathStrategy* pIPathStragey /*= NULL*/ )
	{
		ListIDPath::const_iterator cstItor = m_listIDPath.find(uID);
		if (m_listIDPath.end() != cstItor)
		{
			return (cstItor->second);
		}

		CVRPath* pPath = new CVRPath(uID, strName);
		m_listIDPath[uID] = pPath;

		// 设置集合体
		pPath->SetFeatureGeomtry(pIFeatureGeomtry);
		// 设置策略
		pPath->SetPathStrategy(pIPathStragey);
		return (pPath);
	}

	// 移除路径
	void CPathManager::Remove( const uint64_t uID )
	{
		ListIDPath::iterator itor = m_listIDPath.find(uID);
		if (m_listIDPath.end() != itor)
		{
			VRSOFT_DELETE(itor->second);
			m_listIDPath.erase(itor);
		}
	}

	// 查询路径
	IPath* CPathManager::GetPath( const uint64_t uID ) const
	{
		ListIDPath::const_iterator cstItor = m_listIDPath.find(uID);
		if (m_listIDPath.end() != cstItor)
		{
			return (cstItor->second);
		}

		return (NULL);
	}

	// 查询路径通过名称
	std::vector<IPath*> CPathManager::GetPath( const VRString& strName ) const
	{
		std::vector<IPath*> listPaths;
		ListIDPath::const_iterator cstItor = m_listIDPath.begin();
		for (; m_listIDPath.end() != cstItor; ++cstItor)
		{
			const VRString& str = cstItor->second->GetName();
			if (0 == str.compare(strName))
			{
				listPaths.push_back(cstItor->second);
			}
		}

		return (listPaths);
	}

	// 获得所有路径
	std::vector<IPath*> CPathManager::GetAllPath( void ) const
	{
		std::vector<IPath*> listPaths;
		ListIDPath::const_iterator cstItor = m_listIDPath.begin();
		for (; m_listIDPath.end() != cstItor; ++cstItor)
		{
			listPaths.push_back(cstItor->second);
		}

		return (listPaths);
	}

	// 移除所有的
	void CPathManager::RemoveAll( void )
	{
		ListIDPath::iterator itor = m_listIDPath.begin();
		for (; m_listIDPath.end() != itor; ++itor)
		{
			VRSOFT_DELETE(itor->second);
		}

		m_listIDPath.clear();
	}

}
