#ifndef __BASEFUN_H__
#define __BASEFUN_H__

#include <vector>
#include <string>
using namespace std;
extern "C" char *cltrim( char *str );
extern "C" char *crtrim( char *str );
extern "C" char *ctrimall( char *str );

class QueueArray
{
public:
	QueueArray( ) {  };
	QueueArray( int row, int col );
	~QueueArray( );
public:
	
public:
	// 初始化队列
	void   InitQueue( );
	// 插入队列
	int    EnQueue( const char *data );
	int    EnQueue( string data );
	// 访问队列
	char * VisQueue( int index );

public:
	void   SetRows( int rows );
	int    GetRows( ) { return m_row; }
	void   SetCols( int cols );
	int    GetCols( ) { return m_col; }
	void   ConstrObject( );
	int    GetNowRow();
	string VisQueueStr( int index );

public:
	void   SetAddrAsIndex( int index, const char *src, int len );
	char * GetAddrAsIndex( int index );

private:
	int     m_row;
	int     m_col;
	int     m_nowRow;
	char  **m_array;
	char  **m_addrs;
	vector <string> m_senData;
};


#endif // __BASEFUN_H__