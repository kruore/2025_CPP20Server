#pragma once

// 여기에 미리 컴파일하려는 헤더 추가
#ifdef _DEBUG
#pragma comment(lib,"Debug\\ServerCore.lib") 

#else
#pragma comment(lib,"Release\\ServerCore.lib") 
#endif
#include "CorePch.h"

