#ifndef _WATCHER_SET_H_
#define _WATCHER_SET_H_

#include <unordered_map>
#include <vector>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "watch.h"
#include "watcher.h"

namespace sharkstore {
namespace dataserver {
namespace watch {

typedef std::shared_ptr<Watcher> WatcherPtr;

typedef std::unordered_map<WatcherId, WatcherPtr> KeyWatcherMap;
typedef std::unordered_map<Key, KeyWatcherMap*> WatcherMap;
typedef std::priority_queue<WatcherPtr, std::vector<WatcherPtr>, Greater<WatcherPtr>> WatcherQueue;

typedef std::unordered_map<Key, nullptr_t> WatcherKeyMap;
typedef std::unordered_map<WatcherId, WatcherKeyMap*> KeyMap;

class WatcherSet {
public:
    WatcherSet();
    ~WatcherSet();

    WatchCode AddKeyWatcher(const Key&, WatcherPtr&);
    WatchCode DelKeyWatcher(const Key&, WatcherId);
    WatchCode GetKeyWatchers(std::vector<WatcherPtr>& , const Key&);
    WatchCode AddPrefixWatcher(const Prefix&, WatcherPtr&);
    WatchCode DelPrefixWatcher(const Prefix&, WatcherId);
    WatchCode GetPrefixWatchers(std::vector<WatcherPtr>& , const Prefix&);

private:
    WatcherMap              key_watcher_map_;
    KeyMap                  key_map_;
    WatcherMap              prefix_watcher_map_;
    KeyMap                  prefix_map_;
    WatcherQueue            watcher_queue_;
    std::mutex              watcher_mutex_;

    std::thread                     watcher_timer_;
    volatile bool                   watcher_timer_continue_flag_ = true;
    std::condition_variable         watcher_expire_cond_;

private:
    WatchCode AddWatcher(const Key&, WatcherPtr&, WatcherMap&, KeyMap&);
    WatchCode DelWatcher(const Key&, WatcherId, WatcherMap&, KeyMap&);
    WatchCode GetWatchers(std::vector<WatcherPtr>& vec, const Key&, WatcherMap&);
};


} // namespace watch
}
}

#endif