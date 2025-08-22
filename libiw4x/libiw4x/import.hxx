#pragma once

#include <cstdint>

// clang-format off

namespace iw4x
{
  namespace winmm
  {
    constexpr uintptr_t waveingetnumdevs_slot = 0x140387A10;
    constexpr uintptr_t mixergetnumdevs_slot = 0x140387A18;
    constexpr uintptr_t mixeropen_slot = 0x140387A20;
    constexpr uintptr_t mixerclose_slot = 0x140387A28;
    constexpr uintptr_t timebeginperiod_slot = 0x140387A30;
    constexpr uintptr_t mixersetcontroldetails_slot = 0x140387A38;
    constexpr uintptr_t mixergetcontroldetailsa_slot = 0x140387A40;
    constexpr uintptr_t mixergetlinecontrolsa_slot = 0x140387A48;
    constexpr uintptr_t timegettime_slot = 0x140387A50;
    constexpr uintptr_t timeendperiod_slot = 0x140387A58;
    constexpr uintptr_t mixergetlineinfoa_slot = 0x140387A60;
  }

  namespace wsock32
  {
    constexpr uintptr_t ntohl_slot = 0x140387AB0;
    constexpr uintptr_t inet_addr_slot = 0x140387AB8;
    constexpr uintptr_t gethostbyname_slot = 0x140387AC0;
    constexpr uintptr_t wsagetlasterror_slot = 0x140387AC8;
    constexpr uintptr_t bind_slot = 0x140387AD0;
    constexpr uintptr_t htons_slot = 0x140387AD8;
    constexpr uintptr_t ntohs_slot = 0x140387AE0;
    constexpr uintptr_t select_slot = 0x140387AE8;
    constexpr uintptr_t closesocket_slot = 0x140387AF0;
    constexpr uintptr_t wsastartup_slot = 0x140387AF8;
    constexpr uintptr_t connect_slot = 0x140387B00;
    constexpr uintptr_t ioctlsocket_slot = 0x140387B08;
    constexpr uintptr_t recv_slot = 0x140387B10;
    constexpr uintptr_t recvfrom_slot = 0x140387B18;
    constexpr uintptr_t gethostname_slot = 0x140387B20;
    constexpr uintptr_t socket_slot = 0x140387B28;
    constexpr uintptr_t setsockopt_slot = 0x140387B30;
    constexpr uintptr_t sendto_slot = 0x140387B38;
    constexpr uintptr_t send_slot = 0x140387B40;
    constexpr uintptr_t __wsafdisset_slot = 0x140387B48;
  }

  namespace bink2w64
  {
    constexpr uintptr_t binkgetframebuffersinfo_slot = 0x1403880C0;
    constexpr uintptr_t binkwait_slot = 0x1403880C8;
    constexpr uintptr_t binkseterror_slot = 0x1403880D0;
    constexpr uintptr_t binksetsoundsystem_slot = 0x1403880D8;
    constexpr uintptr_t binksetsoundtrack_slot = 0x1403880E0;
    constexpr uintptr_t binkgetrealtime_slot = 0x1403880E8;
    constexpr uintptr_t binkcontrolbackgroundio_slot = 0x1403880F0;
    constexpr uintptr_t binkgetrects_slot = 0x1403880F8;
    constexpr uintptr_t binkopenxaudio2_slot = 0x140388100;
    constexpr uintptr_t binksetiosize_slot = 0x140388108;
    constexpr uintptr_t binksetspeakervolumes_slot = 0x140388110;
    constexpr uintptr_t binknextframe_slot = 0x140388118;
    constexpr uintptr_t binkdoframe_slot = 0x140388120;
    constexpr uintptr_t binkregisterframebuffers_slot = 0x140388128;
    constexpr uintptr_t binkpause_slot = 0x140388130;
    constexpr uintptr_t binkclose_slot = 0x140388138;
    constexpr uintptr_t binkgeterror_slot = 0x140388140;
    constexpr uintptr_t binkopen_slot = 0x140388148;
    constexpr uintptr_t binksetmemory_slot = 0x140388150;
  }

  namespace d3d9
  {
    constexpr uintptr_t direct3dcreate9_slot = 0x140388160;
    constexpr uintptr_t d3dperf_endevent_slot = 0x140388168;
    constexpr uintptr_t d3dperf_beginevent_slot = 0x140388170;
  }

  namespace dsound
  {
    constexpr uintptr_t __directsoundcapturecreate_slot = 0x140387090;
    constexpr uintptr_t __directsoundcreate8_slot = 0x140387098;
  }

  namespace kernel32
  {
    constexpr uintptr_t releasesrwlockexclusive_slot = 0x1403870D0;
    constexpr uintptr_t acquiresrwlockexclusive_slot = 0x1403870D8;
    constexpr uintptr_t loadlibraryexa_slot = 0x1403870E0;
    constexpr uintptr_t multibytetowidechar_slot = 0x1403870E8;
    constexpr uintptr_t widechartomultibyte_slot = 0x1403870F0;
    constexpr uintptr_t getfilesize_slot = 0x1403870F8;
    constexpr uintptr_t readfileex_slot = 0x140387100;
    constexpr uintptr_t closehandle_slot = 0x140387108;
    constexpr uintptr_t getlasterror_slot = 0x140387110;
    constexpr uintptr_t sleepex_slot = 0x140387118;
    constexpr uintptr_t debugbreak_slot = 0x140387120;
    constexpr uintptr_t createdirectorya_slot = 0x140387128;
    constexpr uintptr_t createfilea_slot = 0x140387130;
    constexpr uintptr_t deletefilea_slot = 0x140387138;
    constexpr uintptr_t getfiletime_slot = 0x140387140;
    constexpr uintptr_t readfile_slot = 0x140387148;
    constexpr uintptr_t writefile_slot = 0x140387150;
    constexpr uintptr_t movefilea_slot = 0x140387158;
    constexpr uintptr_t duplicatehandle_slot = 0x140387160;
    constexpr uintptr_t raiseexception_slot = 0x140387168;
    constexpr uintptr_t setevent_slot = 0x140387170;
    constexpr uintptr_t resetevent_slot = 0x140387178;
    constexpr uintptr_t waitforsingleobject_slot = 0x140387180;
    constexpr uintptr_t createeventa_slot = 0x140387188;
    constexpr uintptr_t sleep_slot = 0x140387190;
    constexpr uintptr_t getcurrentprocess_slot = 0x140387198;
    constexpr uintptr_t createthread_slot = 0x1403871A0;
    constexpr uintptr_t getcurrentthread_slot = 0x1403871A8;
    constexpr uintptr_t getcurrentthreadid_slot = 0x1403871B0;
    constexpr uintptr_t setthreadpriority_slot = 0x1403871B8;
    constexpr uintptr_t getthreadpriority_slot = 0x1403871C0;
    constexpr uintptr_t suspendthread_slot = 0x1403871C8;
    constexpr uintptr_t resumethread_slot = 0x1403871D0;
    constexpr uintptr_t getprocessaffinitymask_slot = 0x1403871D8;
    constexpr uintptr_t setthreadaffinitymask_slot = 0x1403871E0;
    constexpr uintptr_t virtualalloc_slot = 0x1403871E8;
    constexpr uintptr_t virtualfree_slot = 0x1403871F0;
    constexpr uintptr_t queryperformancecounter_slot = 0x1403871F8;
    constexpr uintptr_t queryperformancefrequency_slot = 0x140387200;
    constexpr uintptr_t initializecriticalsection_slot = 0x140387208;
    constexpr uintptr_t entercriticalsection_slot = 0x140387210;
    constexpr uintptr_t leavecriticalsection_slot = 0x140387218;
    constexpr uintptr_t getmodulefilenamea_slot = 0x140387220;
    constexpr uintptr_t getmodulehandlea_slot = 0x140387228;
    constexpr uintptr_t getnativesysteminfo_slot = 0x140387230;
    constexpr uintptr_t getprocaddress_slot = 0x140387238;
    constexpr uintptr_t globalmemorystatus_slot = 0x140387240;
    constexpr uintptr_t setprocessaffinitymask_slot = 0x140387248;
    constexpr uintptr_t getcurrentdirectorya_slot = 0x140387250;
    constexpr uintptr_t seterrormode_slot = 0x140387258;
    constexpr uintptr_t getcurrentprocessid_slot = 0x140387260;
    constexpr uintptr_t createprocessa_slot = 0x140387268;
    constexpr uintptr_t openprocess_slot = 0x140387270;
    constexpr uintptr_t getversionexa_slot = 0x140387278;
    constexpr uintptr_t globalsize_slot = 0x140387280;
    constexpr uintptr_t globalunlock_slot = 0x140387288;
    constexpr uintptr_t globallock_slot = 0x140387290;
    constexpr uintptr_t formatmessagea_slot = 0x140387298;
    constexpr uintptr_t createtoolhelp32snapshot_slot = 0x1403872A0;
    constexpr uintptr_t module32first_slot = 0x1403872A8;
    constexpr uintptr_t module32next_slot = 0x1403872B0;
    constexpr uintptr_t getsystemtime_slot = 0x1403872B8;
    constexpr uintptr_t systemtimetofiletime_slot = 0x1403872C0;
    constexpr uintptr_t muldiv_slot = 0x1403872C8;
    constexpr uintptr_t setpriorityclass_slot = 0x1403872D0;
    constexpr uintptr_t setthreadexecutionstate_slot = 0x1403872D8;
    constexpr uintptr_t freelibrary_slot = 0x1403872E0;
    constexpr uintptr_t loadlibraryexw_slot = 0x1403872E8;
    constexpr uintptr_t getsystemtimeasfiletime_slot = 0x1403872F0;
    constexpr uintptr_t filetimetosystemtime_slot = 0x1403872F8;
    constexpr uintptr_t gettimeformatex_slot = 0x140387300;
    constexpr uintptr_t getdateformatex_slot = 0x140387308;
    constexpr uintptr_t getuserdefaultlocalename_slot = 0x140387310;
    constexpr uintptr_t setcriticalsectionspincount_slot = 0x140387318;
    constexpr uintptr_t deletecriticalsection_slot = 0x140387320;
    constexpr uintptr_t localfree_slot = 0x140387328;
    constexpr uintptr_t globalmemorystatusex_slot = 0x140387330;
    constexpr uintptr_t rtlcapturecontext_slot = 0x140387338;
    constexpr uintptr_t rtllookupfunctionentry_slot = 0x140387340;
    constexpr uintptr_t rtlvirtualunwind_slot = 0x140387348;
    constexpr uintptr_t unhandledexceptionfilter_slot = 0x140387350;
    constexpr uintptr_t initializesrwlock_slot = 0x140387358;
    constexpr uintptr_t sleepconditionvariablecs_slot = 0x140387360;
    constexpr uintptr_t wakeallconditionvariable_slot = 0x140387368;
    constexpr uintptr_t wakeconditionvariable_slot = 0x140387370;
    constexpr uintptr_t initializeconditionvariable_slot = 0x140387378;
    constexpr uintptr_t globalfree_slot = 0x140387380;
    constexpr uintptr_t loadlibraryw_slot = 0x140387388;
    constexpr uintptr_t outputdebugstringa_slot = 0x140387390;
    constexpr uintptr_t setunhandledexceptionfilter_slot = 0x140387398;
    constexpr uintptr_t terminateprocess_slot = 0x1403873A0;
    constexpr uintptr_t isprocessorfeaturepresent_slot = 0x1403873A8;
    constexpr uintptr_t initializecriticalsectionandspincount_slot = 0x1403873B0;
    constexpr uintptr_t waitforsingleobjectex_slot = 0x1403873B8;
    constexpr uintptr_t createeventw_slot = 0x1403873C0;
    constexpr uintptr_t getmodulehandlew_slot = 0x1403873C8;
    constexpr uintptr_t isdebuggerpresent_slot = 0x1403873D0;
    constexpr uintptr_t getstartupinfow_slot = 0x1403873D8;
    constexpr uintptr_t initializeslisthead_slot = 0x1403873E0;
  }

  namespace advapi32
  {
    constexpr uintptr_t eventwritetransfer_slot = 0x140387000;
    constexpr uintptr_t regclosekey_slot = 0x140387008;
    constexpr uintptr_t regopenkeyw_slot = 0x140387010;
    constexpr uintptr_t regqueryvalueexw_slot = 0x140387018;
    constexpr uintptr_t regopenkeya_slot = 0x140387020;
    constexpr uintptr_t regqueryvalueexa_slot = 0x140387028;
    constexpr uintptr_t eventregister_slot = 0x140387030;
    constexpr uintptr_t eventunregister_slot = 0x140387038;
    constexpr uintptr_t eventsetinformation_slot = 0x140387040;
  }

  namespace powrprof
  {
    constexpr uintptr_t callntpowerinformation_slot = 0x1403876C0;
  }

  namespace x3daudio1_7
  {
    constexpr uintptr_t x3daudioinitialize_slot = 0x140387B58;
    constexpr uintptr_t x3daudiocalculate_slot = 0x140387B60;
  }

  namespace xcurl
  {
    constexpr uintptr_t curl_multi_poll_slot = 0x140387B70;
    constexpr uintptr_t curl_multi_cleanup_slot = 0x140387B78;
    constexpr uintptr_t curl_easy_strerror_slot = 0x140387B80;
    constexpr uintptr_t curl_easy_getinfo_slot = 0x140387B88;
    constexpr uintptr_t curl_easy_unescape_slot = 0x140387B90;
    constexpr uintptr_t curl_free_slot = 0x140387B98;
    constexpr uintptr_t curl_slist_free_all_slot = 0x140387BA0;
    constexpr uintptr_t curl_multi_info_read_slot = 0x140387BA8;
    constexpr uintptr_t curl_easy_init_slot = 0x140387BB0;
    constexpr uintptr_t curl_easy_setopt_slot = 0x140387BB8;
    constexpr uintptr_t curl_easy_cleanup_slot = 0x140387BC0;
    constexpr uintptr_t curl_multi_init_slot = 0x140387BC8;
    constexpr uintptr_t curl_multi_add_handle_slot = 0x140387BD0;
    constexpr uintptr_t curl_multi_perform_slot = 0x140387BD8;
    constexpr uintptr_t curl_multi_remove_handle_slot = 0x140387BE0;
    constexpr uintptr_t curl_global_init_slot = 0x140387BE8;
    constexpr uintptr_t curl_slist_append_slot = 0x140387BF0;
    constexpr uintptr_t curl_global_cleanup_slot = 0x140387BF8;
    constexpr uintptr_t curl_easy_escape_slot = 0x140387C00;
  }

  namespace user32
  {
    constexpr uintptr_t adjustwindowrectex_slot = 0x1403876E8;
    constexpr uintptr_t monitorfrompoint_slot = 0x1403876F0;
    constexpr uintptr_t enumdisplaymonitors_slot = 0x1403876F8;
    constexpr uintptr_t wsprintfw_slot = 0x140387700;
    constexpr uintptr_t messageboxw_slot = 0x140387708;
    constexpr uintptr_t getdc_slot = 0x140387710;
    constexpr uintptr_t getmonitorinfoa_slot = 0x140387718;
    constexpr uintptr_t monitorfromwindow_slot = 0x140387720;
    constexpr uintptr_t systemparametersinfoa_slot = 0x140387728;
    constexpr uintptr_t callnexthookex_slot = 0x140387730;
    constexpr uintptr_t unhookwindowshookex_slot = 0x140387738;
    constexpr uintptr_t setwindowshookexa_slot = 0x140387740;
    constexpr uintptr_t mapvirtualkeya_slot = 0x140387748;
    constexpr uintptr_t getasynckeystate_slot = 0x140387750;
    constexpr uintptr_t movewindow_slot = 0x140387758;
    constexpr uintptr_t registerwindowmessagea_slot = 0x140387760;
    constexpr uintptr_t setwindowtexta_slot = 0x140387768;
    constexpr uintptr_t closewindow_slot = 0x140387770;
    constexpr uintptr_t callwindowproca_slot = 0x140387778;
    constexpr uintptr_t postquitmessage_slot = 0x140387780;
    constexpr uintptr_t loadimagea_slot = 0x140387788;
    constexpr uintptr_t adjustwindowrect_slot = 0x140387790;
    constexpr uintptr_t updatewindow_slot = 0x140387798;
    constexpr uintptr_t getsystemmetrics_slot = 0x1403877A0;
    constexpr uintptr_t iswindow_slot = 0x1403877A8;
    constexpr uintptr_t destroywindow_slot = 0x1403877B0;
    constexpr uintptr_t createwindowexa_slot = 0x1403877B8;
    constexpr uintptr_t registerclassa_slot = 0x1403877C0;
    constexpr uintptr_t defwindowproca_slot = 0x1403877C8;
    constexpr uintptr_t sendmessagea_slot = 0x1403877D0;
    constexpr uintptr_t loadicona_slot = 0x1403877D8;
    constexpr uintptr_t loadcursora_slot = 0x1403877E0;
    constexpr uintptr_t getclipboarddata_slot = 0x1403877E8;
    constexpr uintptr_t closeclipboard_slot = 0x1403877F0;
    constexpr uintptr_t openclipboard_slot = 0x1403877F8;
    constexpr uintptr_t showwindow_slot = 0x140387800;
    constexpr uintptr_t registerclassexa_slot = 0x140387808;
    constexpr uintptr_t peekmessagea_slot = 0x140387810;
    constexpr uintptr_t dispatchmessagea_slot = 0x140387818;
    constexpr uintptr_t translatemessage_slot = 0x140387820;
    constexpr uintptr_t getmessagea_slot = 0x140387828;
    constexpr uintptr_t screentoclient_slot = 0x140387830;
    constexpr uintptr_t clienttoscreen_slot = 0x140387838;
    constexpr uintptr_t getcursorpos_slot = 0x140387840;
    constexpr uintptr_t setcursorpos_slot = 0x140387848;
    constexpr uintptr_t showcursor_slot = 0x140387850;
    constexpr uintptr_t getwindowrect_slot = 0x140387858;
    constexpr uintptr_t getforegroundwindow_slot = 0x140387860;
    constexpr uintptr_t setfocus_slot = 0x140387868;
    constexpr uintptr_t postmessagea_slot = 0x140387870;
    constexpr uintptr_t releasedc_slot = 0x140387878;
    constexpr uintptr_t setwindowpos_slot = 0x140387880;
    constexpr uintptr_t getwindowtexta_slot = 0x140387888;
    constexpr uintptr_t getwindowlonga_slot = 0x140387890;
    constexpr uintptr_t setwindowlonga_slot = 0x140387898;
    constexpr uintptr_t getdesktopwindow_slot = 0x1403878A0;
    constexpr uintptr_t enumthreadwindows_slot = 0x1403878A8;
    constexpr uintptr_t changedisplaysettingsa_slot = 0x1403878B0;
    constexpr uintptr_t getactivewindow_slot = 0x1403878B8;
    constexpr uintptr_t messageboxa_slot = 0x1403878C0;
  }

  namespace gdi32
  {
    constexpr uintptr_t getdevicecaps_slot = 0x1403870A8;
    constexpr uintptr_t createfonta_slot = 0x1403870B0;
    constexpr uintptr_t createsolidbrush_slot = 0x1403870B8;
    constexpr uintptr_t setdevicegammaramp_slot = 0x1403870C0;
  }

  namespace shell32
  {
    constexpr uintptr_t shellexecutea_slot = 0x1403876D0;
    constexpr uintptr_t shellexecutew_slot = 0x1403876D8;
  }

  namespace ole32
  {
    constexpr uintptr_t coinitializeex_slot = 0x140388180;
    constexpr uintptr_t cocreateinstance_slot = 0x140388188;
    constexpr uintptr_t stringfromguid2_slot = 0x140388190;
    constexpr uintptr_t cocreateguid_slot = 0x140388198;
    constexpr uintptr_t couninitialize_slot = 0x1403881A0;
    constexpr uintptr_t cotaskmemfree_slot = 0x1403881A8;
  }

  namespace msvcp140
  {
    constexpr uintptr_t __6__basic_ostream_du__char_traits_d_std___std__qeaaaeav01_pebx_z_slot = 0x1403873F0;
    constexpr uintptr_t __fiopen_std__yapeau_iobuf__pebdhh_z_slot = 0x1403873F8;
    constexpr uintptr_t _seekg___basic_istream_du__char_traits_d_std___std__qeaaaeav12_v__fpos_u_mbstatet___2__z_slot = 0x140387400;
    constexpr uintptr_t _tellp___basic_ostream_du__char_traits_d_std___std__qeaa_av__fpos_u_mbstatet___2_xz_slot = 0x140387408;
    constexpr uintptr_t _write___basic_ostream_du__char_traits_d_std___std__qeaaaeav12_pebd_j_z_slot = 0x140387410;
    constexpr uintptr_t __5__basic_istream_du__char_traits_d_std___std__qeaaaeav01_aeah_z_slot = 0x140387418;
    constexpr uintptr_t _imbue___basic_ios__wu__char_traits__w_std___std__qeaa_avlocale_2_aebv32__z_slot = 0x140387420;
    constexpr uintptr_t __xbad_alloc_std__yaxxz_slot = 0x140387428;
    constexpr uintptr_t __xbad_function_call_std__yaxxz_slot = 0x140387430;
    constexpr uintptr_t __6__basic_ostream_du__char_traits_d_std___std__qeaaaeav01_h_z_slot = 0x140387438;
    constexpr uintptr_t __throw_c_error_std__yaxh_z_slot = 0x140387440;
    constexpr uintptr_t _mtx_unlock_slot = 0x140387448;
    constexpr uintptr_t _mtx_lock_slot = 0x140387450;
    constexpr uintptr_t _mtx_destroy_in_situ_slot = 0x140387458;
    constexpr uintptr_t _mtx_init_in_situ_slot = 0x140387460;
    constexpr uintptr_t _id___codecvt_ddu_mbstatet___std__2v0locale_2_a_slot = 0x140387468;
    constexpr uintptr_t _xsputn___basic_streambuf__wu__char_traits__w_std___std__meaa_jpeb_w_j_z_slot = 0x140387470;
    constexpr uintptr_t _xsgetn___basic_streambuf__wu__char_traits__w_std___std__meaa_jpea_w_j_z_slot = 0x140387478;
    constexpr uintptr_t _uflow___basic_streambuf__wu__char_traits__w_std___std__meaagxz_slot = 0x140387480;
    constexpr uintptr_t _sync___basic_streambuf__wu__char_traits__w_std___std__meaahxz_slot = 0x140387488;
    constexpr uintptr_t _showmanyc___basic_streambuf__wu__char_traits__w_std___std__meaa_jxz_slot = 0x140387490;
    constexpr uintptr_t _setbuf___basic_streambuf__wu__char_traits__w_std___std__meaapeav12_pea_w_j_z_slot = 0x140387498;
    constexpr uintptr_t _imbue___basic_streambuf__wu__char_traits__w_std___std__meaaxaebvlocale_2__z_slot = 0x1403874A0;
    constexpr uintptr_t __unlock___basic_streambuf__wu__char_traits__w_std___std__ueaaxxz_slot = 0x1403874A8;
    constexpr uintptr_t __lock___basic_streambuf__wu__char_traits__w_std___std__ueaaxxz_slot = 0x1403874B0;
    constexpr uintptr_t __0__basic_streambuf_du__char_traits_d_std___std__ieaa_xz_slot = 0x1403874B8;
    constexpr uintptr_t __1__basic_iostream__wu__char_traits__w_std___std__ueaa_xz_slot = 0x1403874C0;
    constexpr uintptr_t __0__basic_iostream__wu__char_traits__w_std___std__qeaa_peav__basic_streambuf__wu__char_traits__w_std___1__z_slot = 0x1403874C8;
    constexpr uintptr_t __6__basic_ostream__wu__char_traits__w_std___std__qeaaaeav01_g_z_slot = 0x1403874D0;
    constexpr uintptr_t _tellg___basic_istream_du__char_traits_d_std___std__qeaa_av__fpos_u_mbstatet___2_xz_slot = 0x1403874D8;
    constexpr uintptr_t _read___basic_istream_du__char_traits_d_std___std__qeaaaeav12_pead_j_z_slot = 0x1403874E0;
    constexpr uintptr_t __1__basic_istream_du__char_traits_d_std___std__ueaa_xz_slot = 0x1403874E8;
    constexpr uintptr_t __0__basic_istream_du__char_traits_d_std___std__qeaa_peav__basic_streambuf_du__char_traits_d_std___1__n_z_slot = 0x1403874F0;
    constexpr uintptr_t _flush___basic_ostream__wu__char_traits__w_std___std__qeaaaeav12_xz_slot = 0x1403874F8;
    constexpr uintptr_t __xout_of_range_std__yaxpebd_z_slot = 0x140387500;
    constexpr uintptr_t _uncaught_exception_std__ya_nxz_slot = 0x140387508;
    constexpr uintptr_t __xlength_error_std__yaxpebd_z_slot = 0x140387510;
    constexpr uintptr_t __1__basic_streambuf_du__char_traits_d_std___std__ueaa_xz_slot = 0x140387518;
    constexpr uintptr_t _sputc___basic_streambuf_du__char_traits_d_std___std__qeaahd_z_slot = 0x140387520;
    constexpr uintptr_t _sputn___basic_streambuf_du__char_traits_d_std___std__qeaa_jpebd_j_z_slot = 0x140387528;
    constexpr uintptr_t __pninc___basic_streambuf_du__char_traits_d_std___std__ieaapeadxz_slot = 0x140387530;
    constexpr uintptr_t __1__basic_ios_du__char_traits_d_std___std__ueaa_xz_slot = 0x140387538;
    constexpr uintptr_t _setstate___basic_ios_du__char_traits_d_std___std__qeaaxh_n_z_slot = 0x140387540;
    constexpr uintptr_t __0__basic_ios_du__char_traits_d_std___std__ieaa_xz_slot = 0x140387548;
    constexpr uintptr_t __osfx___basic_ostream_du__char_traits_d_std___std__qeaaxxz_slot = 0x140387550;
    constexpr uintptr_t __6__basic_ostream_du__char_traits_d_std___std__qeaaaeav01_j_z_slot = 0x140387558;
    constexpr uintptr_t _flush___basic_ostream_du__char_traits_d_std___std__qeaaaeav12_xz_slot = 0x140387560;
    constexpr uintptr_t __0__basic_iostream_du__char_traits_d_std___std__qeaa_peav__basic_streambuf_du__char_traits_d_std___1__z_slot = 0x140387568;
    constexpr uintptr_t __1__basic_iostream_du__char_traits_d_std___std__ueaa_xz_slot = 0x140387570;
    constexpr uintptr_t __lock___basic_streambuf_du__char_traits_d_std___std__ueaaxxz_slot = 0x140387578;
    constexpr uintptr_t __unlock___basic_streambuf_du__char_traits_d_std___std__ueaaxxz_slot = 0x140387580;
    constexpr uintptr_t _imbue___basic_streambuf_du__char_traits_d_std___std__meaaxaebvlocale_2__z_slot = 0x140387588;
    constexpr uintptr_t _setbuf___basic_streambuf_du__char_traits_d_std___std__meaapeav12_pead_j_z_slot = 0x140387590;
    constexpr uintptr_t _showmanyc___basic_streambuf_du__char_traits_d_std___std__meaa_jxz_slot = 0x140387598;
    constexpr uintptr_t _sync___basic_streambuf_du__char_traits_d_std___std__meaahxz_slot = 0x1403875A0;
    constexpr uintptr_t _uflow___basic_streambuf_du__char_traits_d_std___std__meaahxz_slot = 0x1403875A8;
    constexpr uintptr_t _xsgetn___basic_streambuf_du__char_traits_d_std___std__meaa_jpead_j_z_slot = 0x1403875B0;
    constexpr uintptr_t _xsputn___basic_streambuf_du__char_traits_d_std___std__meaa_jpebd_j_z_slot = 0x1403875B8;
    constexpr uintptr_t __0_lockit_std__qeaa_h_z_slot = 0x1403875C0;
    constexpr uintptr_t __1_lockit_std__qeaa_xz_slot = 0x1403875C8;
    constexpr uintptr_t _xtime_get_ticks_slot = 0x1403875D0;
    constexpr uintptr_t _query_perf_counter_slot = 0x1403875D8;
    constexpr uintptr_t _query_perf_frequency_slot = 0x1403875E0;
    constexpr uintptr_t __syserror_map_std__yapebdh_z_slot = 0x1403875E8;
    constexpr uintptr_t __winerror_map_std__yahh_z_slot = 0x1403875F0;
    constexpr uintptr_t __bid_locale_std__qeaa_kxz_slot = 0x1403875F8;
    constexpr uintptr_t _classic_locale_std__saaebv12_xz_slot = 0x140387600;
    constexpr uintptr_t __getgloballocale_locale_std__capeav_locimp_12_xz_slot = 0x140387608;
    constexpr uintptr_t _always_noconv_codecvt_base_std__qeba_nxz_slot = 0x140387610;
    constexpr uintptr_t _in___codecvt_ddu_mbstatet___std__qebahaeau_mbstatet__pebd1aeapebdpead3aeapead_z_slot = 0x140387618;
    constexpr uintptr_t _out___codecvt_ddu_mbstatet___std__qebahaeau_mbstatet__pebd1aeapebdpead3aeapead_z_slot = 0x140387620;
    constexpr uintptr_t _unshift___codecvt_ddu_mbstatet___std__qebahaeau_mbstatet__pead1aeapead_z_slot = 0x140387628;
    constexpr uintptr_t __getcat___codecvt_ddu_mbstatet___std__sa_kpeapebvfacet_locale_2_pebv42__z_slot = 0x140387630;
    constexpr uintptr_t _getloc___basic_streambuf_du__char_traits_d_std___std__qeba_avlocale_2_xz_slot = 0x140387638;
    constexpr uintptr_t __init___basic_streambuf_du__char_traits_d_std___std__ieaaxxz_slot = 0x140387640;
    constexpr uintptr_t __0__basic_streambuf__wu__char_traits__w_std___std__ieaa_xz_slot = 0x140387648;
    constexpr uintptr_t __1__basic_streambuf__wu__char_traits__w_std___std__ueaa_xz_slot = 0x140387650;
    constexpr uintptr_t _sputc___basic_streambuf__wu__char_traits__w_std___std__qeaag_w_z_slot = 0x140387658;
    constexpr uintptr_t _sputn___basic_streambuf__wu__char_traits__w_std___std__qeaa_jpeb_w_j_z_slot = 0x140387660;
    constexpr uintptr_t __pninc___basic_streambuf__wu__char_traits__w_std___std__ieaapea_wxz_slot = 0x140387668;
    constexpr uintptr_t _imbue___basic_ios_du__char_traits_d_std___std__qeaa_avlocale_2_aebv32__z_slot = 0x140387670;
    constexpr uintptr_t __1__basic_ios__wu__char_traits__w_std___std__ueaa_xz_slot = 0x140387678;
    constexpr uintptr_t _setstate___basic_ios__wu__char_traits__w_std___std__qeaaxh_n_z_slot = 0x140387680;
    constexpr uintptr_t __0__basic_ios__wu__char_traits__w_std___std__ieaa_xz_slot = 0x140387688;
    constexpr uintptr_t __0__basic_ostream_du__char_traits_d_std___std__qeaa_peav__basic_streambuf_du__char_traits_d_std___1__n_z_slot = 0x140387690;
    constexpr uintptr_t __1__basic_ostream_du__char_traits_d_std___std__ueaa_xz_slot = 0x140387698;
    constexpr uintptr_t __6__basic_ostream_du__char_traits_d_std___std__qeaaaeav01_i_z_slot = 0x1403876A0;
    constexpr uintptr_t __6__basic_ostream_du__char_traits_d_std___std__qeaaaeav01__k_z_slot = 0x1403876A8;
    constexpr uintptr_t __osfx___basic_ostream__wu__char_traits__w_std___std__qeaaxxz_slot = 0x1403876B0;
  }

  namespace vcruntime140
  {
    constexpr uintptr_t __c_specific_handler_slot = 0x1403878D0;
    constexpr uintptr_t longjmp_slot = 0x1403878D8;
    constexpr uintptr_t __cxxframehandler3_slot = 0x1403878E0;
    constexpr uintptr_t __std_terminate_slot = 0x1403878E8;
    constexpr uintptr_t __std_exception_copy_slot = 0x1403878F0;
    constexpr uintptr_t __std_exception_destroy_slot = 0x1403878F8;
    constexpr uintptr_t _cxxthrowexception_slot = 0x140387900;
    constexpr uintptr_t __std_type_info_compare_slot = 0x140387908;
    constexpr uintptr_t __rtdynamiccast_slot = 0x140387910;
    constexpr uintptr_t __intrinsic_setjmp_slot = 0x140387918;
    constexpr uintptr_t __current_exception_slot = 0x140387920;
    constexpr uintptr_t __current_exception_context_slot = 0x140387928;
    constexpr uintptr_t memcpy_slot = 0x140387930;
    constexpr uintptr_t memset_slot = 0x140387938;
    constexpr uintptr_t memcmp_slot = 0x140387940;
    constexpr uintptr_t strstr_slot = 0x140387948;
    constexpr uintptr_t strchr_slot = 0x140387950;
    constexpr uintptr_t memmove_slot = 0x140387958;
    constexpr uintptr_t _purecall_slot = 0x140387960;
    constexpr uintptr_t memchr_slot = 0x140387968;
  }

  namespace vcruntime140_1
  {
    constexpr uintptr_t __cxxframehandler4_slot = 0x140387978;
  }

  namespace api_ms_win_crt_math_l1_1_0
  {
    constexpr uintptr_t sqrtf_slot = 0x140387D10;
    constexpr uintptr_t powf_slot = 0x140387D18;
    constexpr uintptr_t pow_slot = 0x140387D20;
    constexpr uintptr_t floorf_slot = 0x140387D28;
    constexpr uintptr_t tan_slot = 0x140387D30;
    constexpr uintptr_t exp_slot = 0x140387D38;
    constexpr uintptr_t asinf_slot = 0x140387D40;
    constexpr uintptr_t fmodf_slot = 0x140387D48;
    constexpr uintptr_t cos_slot = 0x140387D50;
    constexpr uintptr_t sin_slot = 0x140387D58;
    constexpr uintptr_t atan2f_slot = 0x140387D60;
    constexpr uintptr_t acos_slot = 0x140387D68;
    constexpr uintptr_t sinf_slot = 0x140387D70;
    constexpr uintptr_t log_slot = 0x140387D78;
    constexpr uintptr_t tanf_slot = 0x140387D80;
    constexpr uintptr_t acosf_slot = 0x140387D88;
    constexpr uintptr_t cosf_slot = 0x140387D90;
    constexpr uintptr_t floor_slot = 0x140387D98;
    constexpr uintptr_t atan2_slot = 0x140387DA0;
    constexpr uintptr_t atanf_slot = 0x140387DA8;
    constexpr uintptr_t ceil_slot = 0x140387DB0;
    constexpr uintptr_t ceilf_slot = 0x140387DB8;
    constexpr uintptr_t logf_slot = 0x140387DC0;
    constexpr uintptr_t __setusermatherr_slot = 0x140387DC8;
    constexpr uintptr_t expf_slot = 0x140387DD0;
    constexpr uintptr_t asin_slot = 0x140387DD8;
    constexpr uintptr_t atan_slot = 0x140387DE0;
    constexpr uintptr_t sqrt_slot = 0x140387DE8;
  }

  namespace api_ms_win_crt_convert_l1_1_0
  {
    constexpr uintptr_t strtoull_slot = 0x140387C28;
    constexpr uintptr_t strtol_slot = 0x140387C30;
    constexpr uintptr_t strtoul_slot = 0x140387C38;
    constexpr uintptr_t atof_slot = 0x140387C40;
    constexpr uintptr_t atoi_slot = 0x140387C48;
    constexpr uintptr_t mbstowcs_slot = 0x140387C50;
    constexpr uintptr_t _wtoi_slot = 0x140387C58;
    constexpr uintptr_t mbstowcs_s_slot = 0x140387C60;
    constexpr uintptr_t wcstombs_s_slot = 0x140387C68;
    constexpr uintptr_t _itoa_slot = 0x140387C70;
  }

  namespace api_ms_win_crt_stdio_l1_1_0
  {
    constexpr uintptr_t __stdio_common_vsnwprintf_s_slot = 0x140387EA0;
    constexpr uintptr_t __stdio_common_vsnprintf_s_slot = 0x140387EA8;
    constexpr uintptr_t ungetc_slot = 0x140387EB0;
    constexpr uintptr_t _getcwd_slot = 0x140387EB8;
    constexpr uintptr_t setvbuf_slot = 0x140387EC0;
    constexpr uintptr_t _fseeki64_slot = 0x140387EC8;
    constexpr uintptr_t __stdio_common_vswprintf_s_slot = 0x140387ED0;
    constexpr uintptr_t __stdio_common_vsprintf_slot = 0x140387ED8;
    constexpr uintptr_t fsetpos_slot = 0x140387EE0;
    constexpr uintptr_t fputc_slot = 0x140387EE8;
    constexpr uintptr_t fflush_slot = 0x140387EF0;
    constexpr uintptr_t fseek_slot = 0x140387EF8;
    constexpr uintptr_t __stdio_common_vfprintf_slot = 0x140387F00;
    constexpr uintptr_t fread_slot = 0x140387F08;
    constexpr uintptr_t fopen_slot = 0x140387F10;
    constexpr uintptr_t fclose_slot = 0x140387F18;
    constexpr uintptr_t __acrt_iob_func_slot = 0x140387F20;
    constexpr uintptr_t _get_stream_buffer_pointers_slot = 0x140387F28;
    constexpr uintptr_t __p__commode_slot = 0x140387F30;
    constexpr uintptr_t fgetc_slot = 0x140387F38;
    constexpr uintptr_t __stdio_common_vsscanf_slot = 0x140387F40;
    constexpr uintptr_t _set_fmode_slot = 0x140387F48;
    constexpr uintptr_t ftell_slot = 0x140387F50;
    constexpr uintptr_t fgetpos_slot = 0x140387F58;
    constexpr uintptr_t fwrite_slot = 0x140387F60;
    constexpr uintptr_t __stdio_common_vsprintf_s_slot = 0x140387F68;
  }

  namespace api_ms_win_crt_string_l1_1_0
  {
    constexpr uintptr_t isalnum_slot = 0x140387F78;
    constexpr uintptr_t toupper_slot = 0x140387F80;
    constexpr uintptr_t _stricmp_slot = 0x140387F88;
    constexpr uintptr_t _strnicmp_slot = 0x140387F90;
    constexpr uintptr_t strncmp_slot = 0x140387F98;
    constexpr uintptr_t isdigit_slot = 0x140387FA0;
    constexpr uintptr_t isupper_slot = 0x140387FA8;
    constexpr uintptr_t isspace_slot = 0x140387FB0;
    constexpr uintptr_t strncat_slot = 0x140387FB8;
    constexpr uintptr_t isalpha_slot = 0x140387FC0;
    constexpr uintptr_t strcpy_s_slot = 0x140387FC8;
    constexpr uintptr_t strcmp_slot = 0x140387FD0;
    constexpr uintptr_t wcstok_s_slot = 0x140387FD8;
    constexpr uintptr_t wcsncat_s_slot = 0x140387FE0;
    constexpr uintptr_t strncpy_slot = 0x140387FE8;
    constexpr uintptr_t tolower_slot = 0x140387FF0;
    constexpr uintptr_t strcspn_slot = 0x140387FF8;
    constexpr uintptr_t strtok_slot = 0x140388000;
    constexpr uintptr_t strspn_slot = 0x140388008;
  }

  namespace api_ms_win_crt_utility_l1_1_0
  {
    constexpr uintptr_t rand_slot = 0x140388038;
    constexpr uintptr_t srand_slot = 0x140388040;
    constexpr uintptr_t qsort_slot = 0x140388048;
  }

  namespace api_ms_win_crt_filesystem_l1_1_0
  {
    constexpr uintptr_t _mkdir_slot = 0x140387C80;
    constexpr uintptr_t remove_slot = 0x140387C88;
    constexpr uintptr_t _findclose_slot = 0x140387C90;
    constexpr uintptr_t _findnext64i32_slot = 0x140387C98;
    constexpr uintptr_t _stat64i32_slot = 0x140387CA0;
    constexpr uintptr_t _unlock_file_slot = 0x140387CA8;
    constexpr uintptr_t _lock_file_slot = 0x140387CB0;
    constexpr uintptr_t _findfirst64i32_slot = 0x140387CB8;
  }

  namespace api_ms_win_crt_heap_l1_1_0
  {
    constexpr uintptr_t _callnewh_slot = 0x140387CC8;
    constexpr uintptr_t realloc_slot = 0x140387CD0;
    constexpr uintptr_t malloc_slot = 0x140387CD8;
    constexpr uintptr_t calloc_slot = 0x140387CE0;
    constexpr uintptr_t _set_new_mode_slot = 0x140387CE8;
    constexpr uintptr_t free_slot = 0x140387CF0;
  }

  namespace api_ms_win_crt_runtime_l1_1_0
  {
    constexpr uintptr_t _invalid_parameter_noinfo_noreturn_slot = 0x140387DF8;
    constexpr uintptr_t _errno_slot = 0x140387E00;
    constexpr uintptr_t _configure_narrow_argv_slot = 0x140387E08;
    constexpr uintptr_t _initialize_narrow_environment_slot = 0x140387E10;
    constexpr uintptr_t _initialize_onexit_table_slot = 0x140387E18;
    constexpr uintptr_t _register_onexit_function_slot = 0x140387E20;
    constexpr uintptr_t _invalid_parameter_noinfo_slot = 0x140387E28;
    constexpr uintptr_t _set_errno_slot = 0x140387E30;
    constexpr uintptr_t exit_slot = 0x140387E38;
    constexpr uintptr_t _cexit_slot = 0x140387E40;
    constexpr uintptr_t _seh_filter_exe_slot = 0x140387E48;
    constexpr uintptr_t _set_app_type_slot = 0x140387E50;
    constexpr uintptr_t _get_narrow_winmain_command_line_slot = 0x140387E58;
    constexpr uintptr_t _initterm_slot = 0x140387E60;
    constexpr uintptr_t _initterm_e_slot = 0x140387E68;
    constexpr uintptr_t _exit_slot = 0x140387E70;
    constexpr uintptr_t _c_exit_slot = 0x140387E78;
    constexpr uintptr_t _register_thread_local_exe_atexit_callback_slot = 0x140387E80;
    constexpr uintptr_t terminate_slot = 0x140387E88;
    constexpr uintptr_t _crt_atexit_slot = 0x140387E90;
  }

  namespace api_ms_win_crt_time_l1_1_0
  {
    constexpr uintptr_t _localtime64_s_slot = 0x140388018;
    constexpr uintptr_t _localtime64_slot = 0x140388020;
    constexpr uintptr_t _time64_slot = 0x140388028;
  }

  namespace api_ms_win_crt_locale_l1_1_0
  {
    constexpr uintptr_t _configthreadlocale_slot = 0x140387D00;
  }

  namespace ddraw
  {
    constexpr uintptr_t directdrawenumerateexa_slot = 0x140387078;
    constexpr uintptr_t directdrawcreateex_slot = 0x140387080;
  }

  namespace ws2_32
  {
    constexpr uintptr_t wsarecvfrom_slot = 0x140387A70;
    constexpr uintptr_t wsasocketa_slot = 0x140387A78;
    constexpr uintptr_t getaddrinfo_slot = 0x140387A80;
    constexpr uintptr_t inet_pton_slot = 0x140387A88;
    constexpr uintptr_t wsasendto_slot = 0x140387A90;
    constexpr uintptr_t freeaddrinfo_slot = 0x140387A98;
    constexpr uintptr_t inet_ntop_slot = 0x140387AA0;
  }

  namespace api_ms_win_core_psm_appnotify_l1_1_0
  {
    constexpr uintptr_t registerappstatechangenotification_slot = 0x140387C10;
    constexpr uintptr_t unregisterappstatechangenotification_slot = 0x140387C18;
  }

  namespace bcrypt
  {
    constexpr uintptr_t bcrypthashdata_slot = 0x140388058;
    constexpr uintptr_t bcryptclosealgorithmprovider_slot = 0x140388060;
    constexpr uintptr_t bcryptcreatehash_slot = 0x140388068;
    constexpr uintptr_t bcryptdestroykey_slot = 0x140388070;
    constexpr uintptr_t bcryptdecrypt_slot = 0x140388078;
    constexpr uintptr_t bcryptfinishhash_slot = 0x140388080;
    constexpr uintptr_t bcryptdestroyhash_slot = 0x140388088;
    constexpr uintptr_t bcryptencrypt_slot = 0x140388090;
    constexpr uintptr_t bcryptgeneratesymmetrickey_slot = 0x140388098;
    constexpr uintptr_t bcryptsetproperty_slot = 0x1403880A0;
    constexpr uintptr_t bcryptopenalgorithmprovider_slot = 0x1403880A8;
    constexpr uintptr_t bcryptgenrandom_slot = 0x1403880B0;
  }

  namespace winhttp
  {
    constexpr uintptr_t winhttpwritedata_slot = 0x140387988;
    constexpr uintptr_t winhttpsettimeouts_slot = 0x140387990;
    constexpr uintptr_t winhttpqueryoption_slot = 0x140387998;
    constexpr uintptr_t winhttpopenrequest_slot = 0x1403879A0;
    constexpr uintptr_t winhttpaddrequestheaders_slot = 0x1403879A8;
    constexpr uintptr_t winhttpquerydataavailable_slot = 0x1403879B0;
    constexpr uintptr_t winhttpconnect_slot = 0x1403879B8;
    constexpr uintptr_t winhttpreceiveresponse_slot = 0x1403879C0;
    constexpr uintptr_t winhttpopen_slot = 0x1403879C8;
    constexpr uintptr_t winhttpclosehandle_slot = 0x1403879D0;
    constexpr uintptr_t winhttpsetoption_slot = 0x1403879D8;
    constexpr uintptr_t winhttpsetstatuscallback_slot = 0x1403879E0;
    constexpr uintptr_t winhttpreaddata_slot = 0x1403879E8;
    constexpr uintptr_t winhttpsendrequest_slot = 0x1403879F0;
    constexpr uintptr_t winhttpcrackurl_slot = 0x1403879F8;
    constexpr uintptr_t winhttpqueryheaders_slot = 0x140387A00;
  }

  namespace crypt32
  {
    constexpr uintptr_t certfreecertificatechain_slot = 0x140387050;
    constexpr uintptr_t certopensystemstorew_slot = 0x140387058;
    constexpr uintptr_t certclosestore_slot = 0x140387060;
    constexpr uintptr_t certfindchaininstore_slot = 0x140387068;
  }
}

// clang-format on
