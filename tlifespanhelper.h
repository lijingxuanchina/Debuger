#ifndef __TLIFESPANHELPER_H_
#define __TLIFESPANHELPER_H_

class TLifeSpanHelper
{
public:
	TLifeSpanHelper();
	~TLifeSpanHelper();
	void setHandle(HANDLE handle);
	void setVoidPoint(void* point);

private:
	HANDLE m_handle = nullptr;
	void* m_voidPoint = nullptr;
};

#endif // __TLIFESPANHELPER_H_
