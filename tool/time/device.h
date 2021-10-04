/**
* Copyright (C) 2007 Contributors
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef SYNC_DEVICE_H
#define SYNC_DEVICE_H

#include "base.h"
#include "sync.h"

#ifndef SYNC_PLAYER

/* configure socket-stack */
#ifdef _WIN32
 #define WIN32_LEAN_AND_MEAN
 #define USE_GETADDRINFO
 #ifndef NOMINMAX
  #define NOMINMAX
 #endif
 #include <winsock2.h>
 #include <ws2tcpip.h>
 #include <windows.h>
 #include <limits.h>
#elif defined(USE_AMITCP)
 #include <sys/socket.h>
 #include <proto/exec.h>
 #include <proto/socket.h>
 #include <netdb.h>
 #define SOCKET int
 #define INVALID_SOCKET -1
 #define select(n,r,w,e,t) WaitSelect(n,r,w,e,t,0)
 #define closesocket(x) CloseSocket(x)
#else
 #include <sys/socket.h>
 #include <sys/time.h>
 #include <netinet/in.h>
 #include <netdb.h>
 #include <unistd.h>
 #define SOCKET int
 #define INVALID_SOCKET -1
 #define closesocket(x) close(x)
#endif

#endif /* !defined(SYNC_PLAYER) */

struct sync_device {
	char *base;
	struct sync_track **tracks;
	size_t num_tracks;

#ifndef SYNC_PLAYER
	int row;
	SOCKET sock;
#endif
	struct sync_io_cb io_cb;
};

#endif /* SYNC_DEVICE_H */
