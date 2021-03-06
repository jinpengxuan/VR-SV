/************************************************************************/
/* 作者： LJ                                                            */
/* 用途:  实体渲染管理接口类	                                        */
/* 时间： 2016-03-19                                                    */
/* 修改时间:                                                            */
/************************************************************************/

#ifndef _PUBLIC_ENTITYDRAWMANAGER_H_
#define _PUBLIC_ENTITYDRAWMANAGER_H_

namespace VR_Soft
{
	class VRSOFT_DLL IEntityDrawManager
	{
	public: 
		// 析构函数
		virtual ~IEntityDrawManager(void) { }
		// 获得实体
		virtual IEntityBase* GetEntity(void) const  = 0;
		// 设置地球体
		virtual void SetMapNode3D(osgEarth::MapNode* pMapNode) = 0;
		// 设置地球体
		virtual void SetMapNode2D(osgEarth::MapNode* pMapNode) = 0;
		// 获得渲染体
		virtual DrawObj* GetDrawObj(void) = 0;
		// 获得渲染体2D 
		virtual DrawObj* GetDrawObj2D(void) = 0;
		// 获得根渲染体
		virtual DrawObj* GetRootDrawObj(void) = 0;
		// 添加渲染体
		virtual void Add(IDrawAttribute* pIDrawAttribute) = 0;
		// 移除渲染属性
		virtual void Remove(IDrawAttribute* pIDrawAttribute) = 0;
		// 移除渲染属性
		virtual void Remove(const VRString& strName)  = 0;
		// 移除所有渲染属性
		virtual void RemoveAll() = 0;
		// 修改实体渲染
		virtual void ChangerMeshDrawAttribute(IDrawAttribute* pIDrawAttribute, const VRString& strName)  = 0;
		// 获得渲染属性
		virtual IDrawAttribute* GetAttribute(const VRString& strName) const  = 0 ;
		// 更新属性
		virtual void Update(void)  = 0;
	};

}

#endif // !_PUBLIC_ENTITYDRAWMANAGER_H_
