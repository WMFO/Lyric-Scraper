#include "Log.h"
#include <iostream>
using namespace std;

Log::Log() {
    normal = NULL;
    error  = NULL;
    dirty  = NULL;
}

Log::Log(logger n, logger e, logger f) {
    normal = n;
    error  = e;
    dirty  = f;
}

int Log::setFunc(logger l, int level) {
    switch (level) {
        case LOG_NORMAL:
            normal = l;
            return 1;
        case LOG_ERROR:
            error = l;
            return 1;
        case LOG_DIRTY:
            dirty = l;
            return 1;
    }
    return 0;
}

int Log::log(string msg, int level) {
    switch (level) {
        case LOG_NORMAL:
            if (normal)
                normal(msg);
            return 1;
        case LOG_ERROR:
            if (error)
                error(msg);
            return 1;
        case LOG_DIRTY:
            if (dirty)
                dirty(msg);
            return 1;
    }
    return 0;
}