/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013 University of California, Los Angeles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Zhenkai Zhu <zhenkai@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef TASK_H
#define TASK_H

#define _OVERRIDE
#ifdef __GNUC__
#if __GNUC_MAJOR >= 4 && __GNUC_MINOR__ >= 7
  #undef _OVERRIDE
  #define _OVERRIDE override
#endif // __GNUC__ version
#endif // __GNUC__

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <sys/time.h>

//////////////////////////////////////////////////
// forward declarations
class Task;
typedef boost::shared_ptr<Task> TaskPtr;

class Scheduler;
typedef boost::shared_ptr<Scheduler> SchedulerPtr;

struct event;
//////////////////////////////////////////////////


/**
 * @brief Base class for a task
 */
class Task : public boost::enable_shared_from_this<Task>
{
public:
  // callback of this task
  typedef boost::function<void ()> Callback;
  // tag identifies this task, should be unique
  typedef std::string Tag;
  // used to match tasks
  typedef boost::function<bool (const TaskPtr &task)> TaskMatcher;

  // Task is associated with Schedulers due to the requirement that libevent event is associated with an libevent event_base
  Task(const Callback &callback, const Tag &tag, const SchedulerPtr &scheduler);
  virtual ~Task();

  virtual void
  run() = 0;

  Tag
  tag() { return m_tag; }

  event *
  ev() { return m_event; }

  timeval *
  tv() { return m_tv; }

  // Task needs to be resetted after the callback is invoked if it is to be schedule again; just for safety
  // it's called by scheduler automatically when addTask or rescheduleTask is called;
  // Tasks should do preparation work here (e.g. set up new delay, etc. )
  virtual void
  reset() = 0;

  // set delay
  // This overrides whatever delay kept in m_tv
  void
  setTv(double delay);

  void
  execute();

protected:
  Callback m_callback;
  Tag m_tag;
  SchedulerPtr m_scheduler;
  bool m_invoked;
  event *m_event;
  timeval *m_tv;
};


#endif // TASK_H
