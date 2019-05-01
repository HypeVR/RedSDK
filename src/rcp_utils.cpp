// Copyright 2017 Arunabh Sharma

#include <chrono>
// #include <boost/date_time.hpp>

#include "red/rcp_api.h"
#include "red/rcp_utils.h"

void *rcp_malloc(size_t bytes)
{
  return malloc(bytes);
}

void rcp_free(void *ptr)
{
  free(ptr);
}

int rcp_rand()
{
  static int seed = 1;
  if (seed)
  {
    srand(time(NULL));
    seed = 0;
  }

  return rand();
}

uint32_t rcp_timestamp()
{
  static bool shouldCalcStart = true;
  // static boost::posix_time::ptime start;
  static std::chrono::steady_clock::time_point start;
  if (shouldCalcStart)
  {
    // start           = boost::posix_time::microsec_clock::local_time();
    start           = std::chrono::steady_clock::now();
    shouldCalcStart = false;
  }
  static std::chrono::steady_clock::time_point now =
      std::chrono::steady_clock::now();
  // boost::posix_time::ptime now =
  //     boost::posix_time::microsec_clock::local_time();
  // boost::posix_time::time_duration diff = now - start;
  std::chrono::duration<double, std::milli> diff = now - start;
  return static_cast<uint32_t>(diff.count());
}

// void rcp_log(rcp_log_t severity,
//             const rcp_camera_connection_t *con,
//             const char *msg)
//{
//  (void)severity;
//  (void)con;
//  printf("%s", msg);
//}

void rcp_mutex_lock(rcp_mutex_t id)
{
  switch (id)
  {
    case RCP_MUTEX_CONNECTION: rcpConnectionMutex.lock(); break;
    case RCP_MUTEX_DISCOVERY: rcpDiscoveryMutex.lock(); break;
    default: break;
  }
}

void rcp_mutex_unlock(rcp_mutex_t id)
{
  switch (id)
  {
    case RCP_MUTEX_CONNECTION: rcpConnectionMutex.unlock(); break;
    case RCP_MUTEX_DISCOVERY: rcpDiscoveryMutex.unlock(); break;
    default: break;
  }
}
