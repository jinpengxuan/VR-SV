/************************************************************************/
/* 作者： LJ                                                            */
/* 用途:  日志系统				                                        */
/* 时间： 2015-05-12                                                    */
/* 修改时间:                                                            */
/************************************************************************/
#ifndef _LOGSYSTEM_H_
#define _LOGSYSTEM_H_

#pragma once

namespace VR_Soft
{
	class CLogManager : public ILogManager
	{
	public:
		// 构造函数
		explicit CLogManager(void);
		// 析构函数
		virtual ~CLogManager();

		// 创建默认日志
		virtual ILog* CreateLog(const VRString& strLog, bool bDefaultLog = false,\
			bool debuggerOutput = true, bool suppressFileOutput = false);
		// 获得默认的系统日志
		virtual ILog* GetDefaultLog(void);
		// 设置当前系统日志 返回原来的系统日志
		virtual ILog* SetDefaultLog(ILog* pLog);

		// 获得系统日志对象
		virtual ILog* GetLog(const VRString& strLog);
		// 销毁日志对象
		virtual void DestoryLog(const VRString& strLog);
		virtual void DestoryLog(ILog* pLog);
		// 写入系统日志
		virtual void SetLogMessage(const VRString& strMsg, UINT uiCode = ERROR_OK, LogMessageLevel lml = LML_NORMAL, bool maskDebug = false);

	protected:
		typedef std::map<VRString, ILog*> MapStrLog;
		MapStrLog m_mapStrLog;


	private:
		ILog* m_pDefaultLog; // 默认错误日志对象

		VR_AUTO_MUTEX
	};
}


#endif // LOGSYSTEM_H
