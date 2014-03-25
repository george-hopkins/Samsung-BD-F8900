#ifndef _S_SINGLETON__H_
#define _S_SINGLETON__H_


///
/// USAGE:
/// class SomeClass : public CSingleton<SomeClass>
///	{
///	private:
///		SomeClass();
///		~SomeClass();
///		friend class CSingleton<SomeClass>;
///		friend class CAutoPtr<SomeClass>;
///		...
///	};
/// SomeClass::Get().func()
///
///	Singleton supports thread safe creation,
/// though accessing to a methods are not thread safe
///


template<typename T>
class CSingleton 
{
protected:
	CSingleton() {}
	~CSingleton() {}
public:
	static T& Get()
	{
		if(m_pObject == 0)
		{
			m_pObject = new T;
		}

		return *m_pObject;
	}

private:
	static T*  m_pObject;
};


template<typename T> T* CSingleton<T>::m_pObject;


#endif
