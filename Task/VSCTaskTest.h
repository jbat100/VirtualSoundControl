
#ifndef _VSC_TASK_TEST_H_
#define _VSC_TASK_TEST_H_

#include "VSC.h"
#include "VSCTask.h"
#include "VSCTaskQueue.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <list>

namespace VSC {
    
    class TaskTest {
        
    public:
        
        typedef boost::shared_ptr<TaskTest> SPtr;
        
        void performTestWithTaskQueue(TaskQueue::SPtr queue);
        
    protected:
        
        virtual void internalPerformTestWithTaskQueue(TaskQueue::SPtr queue);

    private:
        
        static const bool trace = true;
        
    };
    
    
}

#endif // _VSC_TASK_TEST_H_


