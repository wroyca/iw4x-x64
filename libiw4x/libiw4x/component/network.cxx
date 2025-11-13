#include <libiw4x/component/network.hxx>

#include <iostream>

#include <libiw4x/symbol.hxx>
#include <libiw4x/types.hxx>
#include <libiw4x/utility/minhook/hook.hxx>
#include <libiw4x/utility/utility-win32.hxx>

using namespace std;
using namespace iw4x::utility;

namespace iw4x
{
  namespace
  {
    scheduler* sched (nullptr);

    // Internal var
    //
    SOCKET* ip_socket (reinterpret_cast<SOCKET*> (0x1467E8490));
  }

  network::
  network (scheduler& s)
  {
    sched = &s;

    try
    {
      s.post ("com_frame",
              []
      {
        try
        {
          // Create socket.
          //
          SOCKET sock (socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP));

          if (sock == INVALID_SOCKET)
          {
            int err (WSAGetLastError ());
            throw runtime_error (to_string (err));
          }

          // Set non-blocking mode.
          //
          u_long mode (1);
          if (ioctlsocket (sock, FIONBIO, &mode) == SOCKET_ERROR)
          {
            int err (WSAGetLastError ());
            closesocket (sock);
            throw runtime_error (to_string (err));
          }

          // Bind socket.
          //
          // Get net_port dvar to determine which port to bind to.
          //
          dvar_t* net_port_dvar (reinterpret_cast<dvar_t*> (
            Dvar_FindVar ("net_port")));

          if (net_port_dvar == nullptr)
            throw runtime_error ("net_port dvar not found");

          int port (net_port_dvar->current.integer);

          sockaddr_in addr {};
          addr.sin_family = AF_INET;
          addr.sin_addr.s_addr = INADDR_ANY;
          addr.sin_port = htons (port);

          if (::bind (sock,
                      reinterpret_cast<sockaddr*> (&addr),
                      sizeof (addr)) == SOCKET_ERROR)
          {
            int err (WSAGetLastError ());
            closesocket (sock);
            throw runtime_error (to_string (err));
          }

          // Store socket in game's ip_socket.
          //
          *ip_socket = sock;
        }
        catch (const exception& e)
        {
          cerr << "error:" << e.what () << endl;
          exit (1);
        }
      });
    }
    catch (const exception& e)
    {
      cerr << "error:" << e.what () << endl;
      exit (1);
    }
  }
}
