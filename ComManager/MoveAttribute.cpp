#include "../../Public/VRSoft.h"
#include "MoveAttribute.h"

#include <osgEarth/GeoMath>

#include "transformation.h"

double  D2R = (osg::PI/180.0);
double	R2D = (180.0/osg::PI);

namespace VR_Soft
{
	CMoveAttribute::CMoveAttribute( IEntityBase* pIEntityBase )
		:CAttributeImp<IEntityMoveAttribute>(pIEntityBase, "运动属性"),m_nIndex(1),
		m_vVelocity(3.0),m_dMaxVel(5.0),m_dAccel(2),m_dLength(0.0),m_dCurLenth(0.0)
	{

	}

	// 析构函数
	CMoveAttribute::~CMoveAttribute( void )
	{

	}

	// 设置实体速度
	void CMoveAttribute::SetVelocity( const double vVelocity )
	{
		m_vVelocity = vVelocity;
	}

	// 获得实体速度
	double CMoveAttribute::GetVelocity( void ) const
	{
		return (m_vVelocity);
	}

	// 设置最大速度
	void CMoveAttribute::SetMaxVelocity( const double vVelocity )
	{
		m_dMaxVel = m_vVelocity;
	}

	// 初始化开始的值 // 将模型朝向目标位置
	void CMoveAttribute::InitMove( void )
	{

	}

	// 获得最大速度
	double CMoveAttribute::GetMaxVelocity( void ) const
	{
		return m_dMaxVel;
	}

	// 设置加速度
	void CMoveAttribute::SetAcceleration( const double vAcceleration )
	{
		m_dAccel = vAcceleration;
	}

	// 获得加速度
	double CMoveAttribute::GetAcceleration( void ) const
	{
		return (m_dAccel);
	}

	// 设置目标位置
	void CMoveAttribute::SetTragetPosition( const osg::Vec3d& vPos )
	{
		m_vTargetPos = vPos;
		// 获得基本属性
		IEntityDrawAttribute* pIEntityDrawAttribute = GetEntity()->GetEntityDrawAttribute();
		if (NULL == pIEntityDrawAttribute)
		{
			return;
		}

		// 获得位置
		osg::Vec3d vCurPos = pIEntityDrawAttribute->GetPosition();

		osg::Vec3d vWordCurPos;
		osg::Vec3d vWordPos;

		double lat, lon, height;		
		
		static osg::EllipsoidModel em; // questionable. make non-static?

		osg::Vec3d v0, v1;

		//CConvert::LatLongHeightToWord()
		em.convertLatLongHeightToXYZ(D2R * vCurPos.y(), D2R * vCurPos.x(), vCurPos.z(), vWordCurPos.x(), vWordCurPos.y(), vWordCurPos.z());
		em.convertLatLongHeightToXYZ(D2R * m_vTargetPos.y(), D2R * m_vTargetPos.x(), m_vTargetPos.z(), vWordPos.x(), vWordPos.y(), vWordPos.z());

		// 求取方位角
		double dHead = -osgEarth::GeoMath::bearing(D2R * vCurPos.y(), D2R * vCurPos.x(), D2R * m_vTargetPos.y(), D2R * m_vTargetPos.x());

		//仿真步长
		double dStep = 0.2;
		double dDistance = m_vVelocity * 0.0001;

		m_dCurLenth += dDistance;
		if (m_dCurLenth >= m_dLength)
		{
			++m_nIndex;
			osg::Vec3d vs;
			CConvert::LatLongHeightToWord(vPos, vs);

			osg::Vec3d ve = m_pIPath->GetPathPoint(m_nIndex);
			CConvert::LatLongHeightToWord(ve, ve);

			// 计算长度
			m_dLength = (ve - vs).length();
			m_dCurLenth = 0.0;
			
			m_vLastPos = vPos;

			pIEntityDrawAttribute->SetPosition(m_vLastPos);
			return;
		}

		double dLon = 0.0;
		double dLat = 0.0;

		// 计算距离
		osg::Vec3d vStartWordPos;
		CConvert::LatLongHeightToWord(m_vLastPos, vStartWordPos);
		osg::Vec3d vDis = vWordPos - vWordCurPos;
		const double dLengthRate = (m_dCurLenth + dDistance) / m_dLength;

		double dHeight = vPos.z() - vCurPos.z();
		double dSin1 = dHeight / vDis.length();
		double arcCos = asin(dSin1) * R2D;

		vWordCurPos = vStartWordPos + (vWordPos - vStartWordPos) * dLengthRate; // vDis.z();
 		em.convertXYZToLatLongHeight(vWordCurPos.x(), vWordCurPos.y(), vWordCurPos.z(), lat, lon, height);
 		lon = osg::RadiansToDegrees(lon);
 		lat = osg::RadiansToDegrees(lat);

		pIEntityDrawAttribute->SetPosition(osg::Vec3d(lon, lat, height));
		osg::Vec3d Missileprh = osg::Vec3d(arcCos, 0.0, R2D * dHead);
		pIEntityDrawAttribute->SetRotate(Missileprh);

	}

	// 设置当前索引位置
	void CMoveAttribute::SetIndex( const int index )
	{
		m_nIndex = index;
	}

	// 获得当前索引位置
	int CMoveAttribute::GetIndex( void ) const
	{
		return (m_nIndex);
	}

	// 获得目标位置
	osg::Vec3d CMoveAttribute::GetTragetPosition( void ) const
	{
		return (m_vTargetPos);
	}

	// 初始化
	void CMoveAttribute::Init( void )
	{
		DEFINITION_PROPERTY(Velocity, &CMoveAttribute::SetVelocity, &CMoveAttribute::GetVelocity, "速度", "", "m/s", CTRL_SPINBOX);
		DEFINITION_PROPERTY(MaxVel, &CMoveAttribute::SetMaxVelocity, &CMoveAttribute::GetMaxVelocity, "最大速度", "", "m/s", CTRL_SPINBOX);
		DEFINITION_PROPERTY(Accel, &CMoveAttribute::SetAcceleration, &CMoveAttribute::GetAcceleration, "加速度", "", "m/s*s", CTRL_SPINBOX);
	}

	double CMoveAttribute::JudgeAzimuth( double dMissileAngle )
	{
		double dHeading = dMissileAngle;
		if(m_dAngle > 0.0 && m_dAngle <= 90.0)
		{
			if(dMissileAngle >= 0.0 && dMissileAngle <= 90.0)
			{
				if(m_dAngle > dMissileAngle)
				{
					CalcAzimuth(true, dHeading);
				}
				else
				{
					CalcAzimuth(false, dHeading);
				}
			}
			else if(dMissileAngle > 90.0 && dMissileAngle <= 180.0)
			{
				CalcAzimuth(false, dHeading);
			}
			else if(dMissileAngle > 180.0 && dMissileAngle <= 270.0)
			{
				if(dMissileAngle - m_dAngle > 180.0)
				{
					CalcAzimuth(true, dHeading);
				}
				else
				{
					CalcAzimuth(false, dHeading);
				}
			}
			else if(dMissileAngle > 270.0 && dMissileAngle <= 360.0)
			{
				CalcAzimuth(true, dHeading);
			}
		}
		else if(m_dAngle > 90.0 && m_dAngle <= 180.0)
		{
			if(dMissileAngle >= 0.0 && dMissileAngle <= 90.0)
			{
				CalcAzimuth(true, dHeading);
			}
			else if(dMissileAngle > 90.0 && dMissileAngle <= 180.0)
			{
				if(m_dAngle > dMissileAngle)
				{
					CalcAzimuth(true, dHeading);
				}
				else
				{
					CalcAzimuth(false, dHeading);
				}
			}
			else if(dMissileAngle > 180.0 && dMissileAngle <= 270.0)
			{
				CalcAzimuth(false, dHeading);
			}
			else if(dMissileAngle > 270.0 && dMissileAngle <= 360.0)
			{
				if(dMissileAngle - m_dAngle > 180.0)
				{
					CalcAzimuth(true, dHeading);
				}
				else
				{
					CalcAzimuth(false, dHeading);
				}
			}
		}
		else if(m_dAngle > 180.0 && m_dAngle <= 270.0)
		{
			if(dMissileAngle >= 0.0 && dMissileAngle <= 90.0)
			{
				if(m_dAngle - dMissileAngle > 180.0)
				{
					CalcAzimuth(false, dHeading);
				}
				else
				{
					CalcAzimuth(true, dHeading);
				}
			}
			else if(dMissileAngle > 90.0 && dMissileAngle <= 180.0)
			{
				CalcAzimuth(true, dHeading);
			}
			else if(dMissileAngle > 180.0 && dMissileAngle <= 270.0)
			{
				if(m_dAngle > dMissileAngle)
				{
					CalcAzimuth(true, dHeading);
				}
				else
				{
					CalcAzimuth(false, dHeading);
				}
			}
			else if(dMissileAngle > 270.0 && dMissileAngle <= 360.0)
			{
				CalcAzimuth(false, dHeading);
			}
		}
		else if(m_dAngle > 270.0 && m_dAngle <= 360.0)
		{
			if(dMissileAngle >= 0.0 && dMissileAngle <= 90.0)
			{
				CalcAzimuth(false, dHeading);
			}
			else if(dMissileAngle > 90.0 && dMissileAngle <= 180.0)
			{
				if(m_dAngle - dMissileAngle > 180.0)
				{
					CalcAzimuth(false, dHeading);
				}
				else
				{
					CalcAzimuth(true, dHeading);
				}
			}
			else if(dMissileAngle > 180.0 && dMissileAngle <= 270.0)
			{
				CalcAzimuth(true, dHeading);
			}
			else if(dMissileAngle > 270.0 && dMissileAngle <= 360.0)
			{
				if(m_dAngle > dMissileAngle)
				{
					CalcAzimuth(true, dHeading);
				}
				else
				{
					CalcAzimuth(false, dHeading);
				}
			}
		}
		return dHeading;
	}

	double CMoveAttribute::CalcPitch( bool bFlag, double dLon, double dLat )
	{
		double ratio = osg::PI / 180.0;
		double dDis = 0.0;
		Transformation tf;
		tf.calc_two_pnt_dist(dLon, dLat, m_dTargetLon, m_dTargetLat, dDis);
		dDis = dDis*1000.0;
		double dHeightDis = fabs(m_dTargetHeight - m_dMissileHeight);
		double dPitch = atan(dHeightDis / dDis);
		dPitch = dPitch /ratio;
		if(!bFlag)
		{
			dPitch = -dPitch;
		}
		return dPitch;
	}

	void CMoveAttribute::CalcAzimuth( bool bFlag, double& dHead )
	{
		if(m_nSecondCount == m_nAzimuthFlag)
		{
			if(bFlag)
			{
				if(fabs(m_dAngle - dHead) > 2.00000000001)
				{
					dHead += 2.0;
					if(dHead > 360.0)
					{
						dHead = dHead - 360.0;
					}
				}
				else
				{
					dHead = m_dAngle;
				}
			}
			else
			{
				if(fabs(m_dAngle - dHead) > 2.00000000001)
				{
					dHead -= 2.0;
					if(dHead < 0.0)
					{
						dHead = 360.0 + dHead;
					}
				}
				else
				{
					dHead = m_dAngle;
				}
			}
			m_nAzimuthFlag = 0;
		}
		else
		{
			m_nAzimuthFlag++;
		}
	}

	// 更新
	void CMoveAttribute::Update( void )
	{
		// 获得运动当前的点的索引
		if (NULL == m_pIPath)
		{
			return;
		}
		osg::Vec3d vTragetPos = m_pIPath->GetPathPoint(m_nIndex);

		SetTragetPosition(vTragetPos);
	}

	// 设置路径
	void CMoveAttribute::SetMovePath( IPath* pIPath )
	{
		m_pIPath = pIPath;

		// 设置开始位置
		osg::Vec3d vStartPos = m_pIPath->GetPathPoint(0);
		// 获得位置
		IEntityDrawAttribute* pIEntityDrawAttribute = GetEntity()->GetEntityDrawAttribute();
		if (NULL != pIEntityDrawAttribute)
		{
			pIEntityDrawAttribute->SetPosition(vStartPos);
		}

		m_vLastPos = vStartPos;

		osg::Vec3d vs;
		CConvert::LatLongHeightToWord(m_vLastPos, vs);

		osg::Vec3d ve = m_pIPath->GetPathPoint(1);
		CConvert::LatLongHeightToWord(ve, ve);

		// 计算长度
		m_dLength = (ve - vs).length();
	}

	// 获得路径
	IPath* CMoveAttribute::GetMovePath( void ) const
	{
		return (m_pIPath);
	}

	// 解析xml
	void CMoveAttribute::ReadXmlNode( TiXmlElement* pTiXmlElement )
	{
		if (NULL == pTiXmlElement)
		{
			return ;
		}

		// 获得属性
		TiXmlElement* pSubXmlElement = pTiXmlElement->FirstChildElement();
		while (NULL != pSubXmlElement)
		{
			VRString strValue = pSubXmlElement->Value();
			if (0 == strValue.compare("速度"))
			{
				const VRString strText = pSubXmlElement->GetText();
				double dV = CVRStringUtil::StringToNumber<double>(strText);
				GetItem("速度")->SetValue(dV);
			}
			else if (0 == strValue.compare("最大速度"))
			{
				const VRString strText = pSubXmlElement->GetText();
				double dV = CVRStringUtil::StringToNumber<double>(strText);
				GetItem("最大速度")->SetValue(dV);
			}
			else if (0 == strValue.compare("加速度"))
			{
				const VRString strText = pSubXmlElement->GetText();
				double dV = CVRStringUtil::StringToNumber<double>(strText);
				GetItem("加速度")->SetValue(dV);
			}

			// 读取下一个属性
			pSubXmlElement = pSubXmlElement->NextSiblingElement();
		}
	}

	void CMoveAttribute::InterPolate(double lat1Rad, double lon1Rad, double dHeight1,
		double lat2Rad, double lon2Rad, double dHeight2,
		double t,
		double& out_latRad, double& out_lonRad, double &out_Height)
	{
		static osg::EllipsoidModel em; // questionable. make non-static?

		osg::Vec3d v0, v1;

		double dH1, dH2;
		//CConvert::LatLongHeightToWord()
		em.convertLatLongHeightToXYZ(lat1Rad, lon1Rad, dHeight1, v0.x(), v0.y(), v0.z());
		//CConvert::LatLongHeightToWord(lat1Rad, lon1Rad, dHeight1, v0.x(), v0.y(), v0.z());
		double r0 = v0.length();
		dH1 = v0.z();
		v0.normalize();
		em.convertLatLongHeightToXYZ(lat2Rad, lon2Rad, dHeight2, v1.x(), v1.y(), v1.z());
		//CConvert::LatLongHeightToWord(lat2Rad, lon2Rad, dHeight2, v1.x(), v1.y(), v1.z());
		double r1 = v1.length();
		dH2 = v1.z();
		v1.normalize();

		osg::Vec3d axis = v0 ^ v1;
		double angle = acos( v0 * v1 );
		osg::Quat q( angle * t, axis );

		v0 = (q * v0) * 0.5*(r0 + r1);

		double dHeight = dH1 + (dH2 - dH1) * t;

		//double dummy;
		em.convertXYZToLatLongHeight( v0.x(), v0.y(), dHeight, out_latRad, out_lonRad, out_Height );
	}


	
}
