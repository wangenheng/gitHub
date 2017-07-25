#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "basefun.h"

#pragma warning(disable:4786)
char *cltrim( char *str )
{
    char    *pstart, *ptr;

    if ( str == NULL )
        return NULL;

    pstart = ptr = str;

    while ( *( ( unsigned char * )ptr ) == 0x20 || *ptr == '\t' || \
            *ptr == '\r' || *ptr == '\n' || *ptr == '[' || *ptr == '>' )
        ptr ++;

    while ( *ptr != 0x0 )
        *str ++ = *ptr ++;

    *str = 0x0;

    return pstart;
}

char *crtrim( char *str )
{
    char    *ptr;

    if ( str == NULL )
        return NULL;

    ptr = str + strlen( str ) - 1;

    while ( ptr >= str && ( *( ( unsigned char * )ptr ) == 0x20 || \
            *ptr == '\t' ||  *ptr == '\r' || *ptr == '\n' || *ptr == ']' ))
        *ptr -- = 0x0;

    return str;
}

char *ctrimall( char *str )
{
    char *pstart, *ptr;

    if ( str == NULL )
        return NULL;

    pstart = ptr = str;

    while ( *ptr != 0x0 )
        if ( *( ( unsigned char * )ptr ) == 0x20 )
            ptr ++;
        else
            *str ++ = *ptr ++;

    *str = 0x0;

    return pstart;
}

//inline QueueArray::QueueArray( int row, int col )
QueueArray::QueueArray( int row, int col )
{
	int i;
	m_row = row;
	m_col = col;

	m_array = new char*[m_row];
	for (i = 0; i<m_row; i++)
	{
		m_array[i] = new char[m_col];
		memset( m_array[i], 0, m_col );
	}

	m_addrs = new char*[m_row];
	for (i = 0; i<m_row; i++)
	{
		m_addrs[i] = new char[m_col];
		memset( m_addrs[i], 0, m_col );
	}
	m_nowRow = 0;
}

//inline QueueArray::~QueueArray( )
QueueArray::~QueueArray( )
{
	if (m_row != 0 && m_col != 0)
	{
		for (int i=0; i<m_row; i++)
		{
			delete [] m_array[i];
		}

		if (m_array)
		{
			delete [] m_array;
		}
	}

	m_row = m_col = m_nowRow = 0;
	
}

//inline void QueueArray::InitQueue( )
void QueueArray::InitQueue( )
{
	for (int i = 0; i < m_row; i++)
	{
		memset( m_array[i], 0, m_col );
	}
	m_nowRow = 0;
}

//inline int QueueArray::EnQueue( const char* data )
int QueueArray::EnQueue( const char* data )
{
	int i;
	/*
	for (i = 0; i<m_row-1; i++)
	{
		memset( m_array[i], 0, m_col );
		memcpy( m_array[i], m_array[i+1], strlen(m_array[i+1]) );
	}

	memset( m_array[i], 0, m_col );
	memcpy( m_array[i], data, strlen(data) );
	*/
	if( m_nowRow < m_row )
	{
		memset( m_array[m_nowRow], 0, m_col );
		memcpy( m_array[m_nowRow], data, strlen(data) );
		m_nowRow++;
	}
	else 
	{
		for (i = 0; i<m_row-1; i++)
		{
			memset( m_array[i], 0, m_col );
			memcpy( m_array[i], m_array[i+1], strlen(m_array[i+1]) );
		}
		
		memset( m_array[i], 0, m_col );
		memcpy( m_array[i], data, strlen(data) );
	}
	
	return 0;
}
int QueueArray::EnQueue( string data )
{
	int i;
	if( m_nowRow < m_row )
	{
		//memset( m_array[m_nowRow], 0, m_col );
		//memcpy( m_array[m_nowRow], data, strlen(data) );
		m_senData.insert(m_senData.begin()+m_nowRow,data);
		m_nowRow++;
	}
	else 
	{
		for (i = 0; i<m_row-1; i++)
		{
			m_senData[i] = m_senData[i+1];
		}
		
		m_senData[m_row-1] = data;
	}
	
	return 0;
}
//inline char * QueueArray::VisQueue( int index )
char * QueueArray::VisQueue( int index )
{
	return m_array[index];
}

string QueueArray::VisQueueStr( int index )
{
	return m_senData[index];
}

//inline void QueueArray::SetRows( int rows )
void QueueArray::SetRows( int rows )
{
	m_row = rows;
}

//inline void QueueArray::SetCols( int cols )
void QueueArray::SetCols( int cols )
{
	m_col = cols;
}
int QueueArray::GetNowRow( )
{
	return m_nowRow;
}
//inline void QueueArray::ConstrObject( )
void QueueArray::ConstrObject( )
{
	int i;

	m_array = new char*[m_row];
	for (i = 0; i<m_row; i++)
	{
		m_array[i] = new char[m_col];
		memset( m_array[i], 0, m_col );
	}

	m_addrs = new char*[m_row];
	for (i = 0; i<m_row; i++)
	{
		m_addrs[i] = new char[m_col];
		memset( m_addrs[i], 0, m_col );
	}
}

//inline void   QueueArray::SetAddrAsIndex(int index, const char *src, int len)
void   QueueArray::SetAddrAsIndex(int index, const char *src, int len)
{
	if (len > m_col)
		return ;
	memset(m_addrs[index], 0, m_col);
	strncpy(m_addrs[index], src, len);
}

//inline char * QueueArray::GetAddrAsIndex( int index )
char * QueueArray::GetAddrAsIndex( int index )
{
	return m_addrs[index];
}
