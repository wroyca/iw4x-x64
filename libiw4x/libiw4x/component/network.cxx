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

    // Internal socket state.
    //
    struct socket_state
    {
      SOCKET* ip;
      SOCKET* lsp;

      explicit
      socket_state ()
        : ip  ((SOCKET*) 0x1467E8490),
          lsp ((SOCKET*) 0x1467E8498)
      {}
    };

    socket_state st;

    int
    net_send_packet (size_t n, const char* b, netadr_t* a)
    {
      try
      {
        SOCKET s (*st.ip);

        if (s == INVALID_SOCKET || s == 0)
          throw runtime_error ("invalid socket");

        // IW4 always stores four octets in ip[] and keeps port in network byte
        // order.
        //
        sockaddr_in sa;
        sa.sin_family = AF_INET;
        memcpy (&sa.sin_addr, a->ip, 4);
        sa.sin_port = a->port;

        int r (sendto (s,
                       b,
                       static_cast<int> (n),
                       0,
                       (sockaddr*) (&sa),
                       sizeof (sockaddr_in)));

        if (r == SOCKET_ERROR)
          throw runtime_error (format_message (WSAGetLastError ()));

        cout << "sent " << r << " bytes" << endl;
        return true;
      }
      catch (const exception& e)
      {
        cerr << "error:" << e.what () << endl;
        exit (1);
      }
    }
  }

  network::
  network (scheduler& s)
  {
    sched = &s;

    minhook::create (NET_SendPacket, &net_send_packet);

    try
    {
      s.post ("com_frame",
              []
      {
        try
        {
          SOCKET s (socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP));

          if (s == INVALID_SOCKET)
            throw runtime_error (format_message (WSAGetLastError ()));

          // Switch the socket into non-blocking mode.
          //
          if (u_long m (1); ioctlsocket (s, FIONBIO, &m) == SOCKET_ERROR)
            throw runtime_error (format_message (WSAGetLastError ()));

          sockaddr_in sa {};
          sa.sin_family = AF_INET;
          sa.sin_addr.s_addr = INADDR_ANY;
          sa.sin_port = htons (0);

          if (::bind (s, (sockaddr*) &sa, sizeof (sa)) == SOCKET_ERROR)
            throw runtime_error (format_message (WSAGetLastError ()));

          // Print bound port (ephemeral, assigned by OS).
          //
          // Used only for debugging: the manual connect sequence later requires
          // the actual local port.
          //
          sockaddr_in ba {};
          int al (sizeof (ba));

          if (getsockname (s, (sockaddr*) &ba, &al) == 0)
            cout << "bound to: " << ntohs (ba.sin_port) << endl;

          // Store socket in game's internal ip_socket.
          //
          *st.ip = s;
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
