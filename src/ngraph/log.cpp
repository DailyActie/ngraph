/*******************************************************************************
* Copyright 2017-2018 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include <chrono>
#include <condition_variable>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <thread>

#include "ngraph/log.hpp"

using namespace std;

namespace ngraph
{
    class thread_starter;
}

string ngraph::logger::log_path;
deque<string> ngraph::logger::queue;
static mutex queue_mutex;
static condition_variable queue_condition;
static unique_ptr<thread> queue_thread;
static bool active = false;

std::ostream& ngraph::get_nil_stream()
{
    static std::stringstream nil;
    return nil;
}

class ngraph::thread_starter
{
public:
    thread_starter() { ngraph::logger::start(); }
    virtual ~thread_starter() { ngraph::logger::stop(); }
};

static ngraph::thread_starter _starter;

void ngraph::logger::set_log_path(const string& path)
{
    log_path = path;
}

void ngraph::logger::start()
{
    active = true;
    queue_thread = unique_ptr<thread>(new thread(&thread_entry, nullptr));
}

void ngraph::logger::stop()
{
    {
        unique_lock<std::mutex> lk(queue_mutex);
        active = false;
        queue_condition.notify_one();
    }
    queue_thread->join();
}

void ngraph::logger::process_event(const string& s)
{
    cout << s << "\n";
}

void ngraph::logger::thread_entry(void* param)
{
    unique_lock<std::mutex> lk(queue_mutex);
    while (active)
    {
        queue_condition.wait(lk);
        while (!queue.empty())
        {
            process_event(queue.front());
            queue.pop_front();
        }
    }
}

void ngraph::logger::log_item(const string& s)
{
    unique_lock<std::mutex> lk(queue_mutex);
    queue.push_back(s);
    queue_condition.notify_one();
}

ngraph::log_helper::log_helper(LOG_TYPE type, const char* file, int line, const char* func)
{
    switch (type)
    {
    case LOG_TYPE::_LOG_TYPE_ERROR: _stream << "[ERR ] "; break;
    case LOG_TYPE::_LOG_TYPE_WARNING: _stream << "[WARN] "; break;
    case LOG_TYPE::_LOG_TYPE_INFO: _stream << "[INFO] "; break;
    case LOG_TYPE::_LOG_TYPE_DEBUG: _stream << "[DEBUG] "; break;
    }

    std::time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now());
    auto tm = std::gmtime(&tt);
    char buffer[256];
    //    strftime(buffer,sizeof(buffer), "%d/%b/%Y:%H:%M:%S %z", tm);
    //    strftime(buffer,sizeof(buffer), "%Y-%m-%d %H:%M:%S UTC", tm);
    strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%Sz", tm);
    _stream << buffer << " ";

    _stream << file;
    _stream << " " << line;
    //    _stream << " " << func;
    _stream << "\t";
}

ngraph::log_helper::~log_helper()
{
    cout << _stream.str() << endl;
    // logger::log_item(_stream.str());
}
