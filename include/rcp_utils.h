// Copyright 2017 Arunabh Sharma

#ifndef INCLUDE_RED_RCP_UTILS_H_
#define INCLUDE_RED_RCP_UTILS_H_

#include <boost/thread.hpp>

#include "red/rcp_api.h"

#ifdef __cplusplus
extern "C"
{
#endif

  boost::recursive_mutex rcpConnectionMutex;
  boost::recursive_mutex rcpDiscoveryMutex;

  void *rcp_malloc(size_t bytes);

  void rcp_free(void *ptr);

  int rcp_rand();

  uint32_t rcp_timestamp();

  // void rcp_log(rcp_log_t severity,
  //             const rcp_camera_connection_t *con,
  //             const char *msg);

  void rcp_mutex_lock(rcp_mutex_t id);

  void rcp_mutex_unlock(rcp_mutex_t id);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_RED_RCP_UTILS_H_
