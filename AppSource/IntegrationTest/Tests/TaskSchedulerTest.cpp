//
//  TaskSchedulerTest.cpp
//  CSTest
//  Created by Ian Copland on 05/04/2016.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <IntegrationTest/TestSystem/TestCase.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Threading.h>


namespace CSTest
{
    namespace IntegrationTest
    {
        //------------------------------------------------------------------------------
        /// A series of integration tests for the Task Scheduler.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        CSIT_TESTCASE(TaskScheduler)
        {
            //------------------------------------------------------------------------------
            /// Tests that a small task can be scheduled on a background thread.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SmallTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();

                taskScheduler->ScheduleTask(CSCore::TaskType::k_small, [=](const CSCore::TaskContext& in_taskContext) noexcept
                {
                    CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_small, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Tests that a large task can be scheduled on a background thread.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(LargeTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();

                taskScheduler->ScheduleTask(CSCore::TaskType::k_large, [=](const CSCore::TaskContext& in_taskContext) noexcept
                {
                    CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_large, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Tests that a task can be scheduled on the main thread.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(MainThreadTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();

                taskScheduler->ScheduleTask(CSCore::TaskType::k_mainThread, [=](const CSCore::TaskContext& in_taskContext) noexcept
                {
                    CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_mainThread, "Incorrect task type.");
                    CSIT_ASSERT(taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Tests that a game logic task can be scheduled on a background thread.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(GameLogicTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();

                taskScheduler->ScheduleTask(CSCore::TaskType::k_gameLogic, [=](const CSCore::TaskContext& in_taskContext) noexcept
                {
                    CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_gameLogic, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Tests that a file task can be scheduled on a background thread.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(FileTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();

                taskScheduler->ScheduleTask(CSCore::TaskType::k_file, [=](const CSCore::TaskContext& in_taskContext) noexcept
                {
                    CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_file, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Tests that a simple small task can be scheduled on a background thread.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SimpleSmallTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                taskScheduler->ScheduleTask(CSCore::TaskType::k_small, [=]() noexcept
                {
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Tests that a simple large task can be scheduled on a background thread.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SimpleLargeTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                taskScheduler->ScheduleTask(CSCore::TaskType::k_large, [=]() noexcept
                {
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Tests that a simple task can be scheduled on the main thread.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SimpleMainThreadTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                taskScheduler->ScheduleTask(CSCore::TaskType::k_mainThread, [=]() noexcept
                {
                    CSIT_ASSERT(taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Tests that a simple game logic task can be scheduled on a background thread.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SimpleGameLogicTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                taskScheduler->ScheduleTask(CSCore::TaskType::k_gameLogic, [=]() noexcept
                {
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Tests that a simple file task can be scheduled on a background thread.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SimpleFileTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                taskScheduler->ScheduleTask(CSCore::TaskType::k_file, [=]() noexcept
                {
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a batch of small tasks can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SmallTaskBatch)
            {
                constexpr u32 k_numTasks = 5;
                
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                
                std::vector<CSCore::TaskSchedulerNew::Task> tasks;
                for (u32 i = 0; i < k_numTasks; ++i)
                {
                    tasks.push_back([=](const CSCore::TaskContext& in_taskContext) noexcept
                    {
                        CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_small, "Incorrect task type.");
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                        
                        ++(*executedCount);
                    });
                }
                
                taskScheduler->ScheduleTasks(CSCore::TaskType::k_small, tasks, [=](const CSCore::TaskContext& in_taskContext) noexcept
                {
                    CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_small, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_ASSERT(*executedCount == k_numTasks, "An incorrect amount of tasks were run.");
                    
                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a batch of large tasks can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(LargeTaskBatch)
            {
                constexpr u32 k_numTasks = 5;
                
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                
                std::vector<CSCore::TaskSchedulerNew::Task> tasks;
                for (u32 i = 0; i < k_numTasks; ++i)
                {
                    tasks.push_back([=](const CSCore::TaskContext& in_taskContext) noexcept
                    {
                        CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_large, "Incorrect task type.");
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedCount);
                    });
                }

                taskScheduler->ScheduleTasks(CSCore::TaskType::k_large, tasks, [=](const CSCore::TaskContext& in_taskContext) noexcept
                {
                    CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_large, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_ASSERT(*executedCount == k_numTasks, "An incorrect amount of tasks were run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a batch of main thread tasks can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(MainThreadTaskBatch)
            {
                constexpr u32 k_numTasks = 5;
                
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                
                std::vector<CSCore::TaskSchedulerNew::Task> tasks;
                for (u32 i = 0; i < k_numTasks; ++i)
                {
                    tasks.push_back([=](const CSCore::TaskContext& in_taskContext) noexcept
                    {
                        CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_mainThread, "Incorrect task type.");
                        CSIT_ASSERT(taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedCount);
                    });
                }
                
                taskScheduler->ScheduleTasks(CSCore::TaskType::k_mainThread, tasks, [=](const CSCore::TaskContext& in_taskContext) noexcept
                {
                    CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_mainThread, "Incorrect task type.");
                    CSIT_ASSERT(taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_ASSERT(*executedCount == k_numTasks, "An incorrect amount of tasks were run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a batch of game logic tasks can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(GameLogicTaskBatch)
            {
                constexpr u32 k_numTasks = 5;
                
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                
                std::vector<CSCore::TaskSchedulerNew::Task> tasks;
                for (u32 i = 0; i < k_numTasks; ++i)
                {
                    tasks.push_back([=](const CSCore::TaskContext& in_taskContext) noexcept
                    {
                        CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_gameLogic, "Incorrect task type.");
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedCount);
                    });
                }
                
                taskScheduler->ScheduleTasks(CSCore::TaskType::k_gameLogic, tasks, [=](const CSCore::TaskContext& in_taskContext) noexcept
                {
                    CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_gameLogic, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_ASSERT(*executedCount == k_numTasks, "An incorrect amount of tasks were run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a batch of file tasks can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(FileTaskBatch)
            {
                constexpr u32 k_numTasks = 5;
                
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                
                std::vector<CSCore::TaskSchedulerNew::Task> tasks;
                for (u32 i = 0; i < k_numTasks; ++i)
                {
                    tasks.push_back([=](const CSCore::TaskContext& in_taskContext) noexcept
                    {
                        CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_file, "Incorrect task type.");
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedCount);
                    });
                }
                
                taskScheduler->ScheduleTasks(CSCore::TaskType::k_file, tasks, [=](const CSCore::TaskContext& in_taskContext) noexcept
                {
                    CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_file, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_ASSERT(*executedCount == k_numTasks, "An incorrect amount of tasks were run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a batch of simple small tasks can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SimpleSmallTaskBatch)
            {
                constexpr u32 k_numTasks = 5;
                
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                
                std::vector<CSCore::TaskSchedulerNew::SimpleTask> tasks;
                for (u32 i = 0; i < k_numTasks; ++i)
                {
                    tasks.push_back([=]() noexcept
                    {
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedCount);
                    });
                }
                
                taskScheduler->ScheduleTasks(CSCore::TaskType::k_small, tasks, [=]() noexcept
                {
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_ASSERT(*executedCount == k_numTasks, "An incorrect amount of tasks were run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a batch of simple large tasks can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SimpleLargeTaskBatch)
            {
                constexpr u32 k_numTasks = 5;
                
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                
                std::vector<CSCore::TaskSchedulerNew::SimpleTask> tasks;
                for (u32 i = 0; i < k_numTasks; ++i)
                {
                    tasks.push_back([=]() noexcept
                    {
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedCount);
                    });
                }
                
                taskScheduler->ScheduleTasks(CSCore::TaskType::k_large, tasks, [=]() noexcept
                {
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_ASSERT(*executedCount == k_numTasks, "An incorrect amount of tasks were run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a batch of simple main thread tasks can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SimpleMainThreadTaskBatch)
            {
                constexpr u32 k_numTasks = 5;
                
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                
                std::vector<CSCore::TaskSchedulerNew::SimpleTask> tasks;
                for (u32 i = 0; i < k_numTasks; ++i)
                {
                    tasks.push_back([=]() noexcept
                    {
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedCount);
                    });
                }
                
                taskScheduler->ScheduleTasks(CSCore::TaskType::k_mainThread, tasks, [=]() noexcept
                {
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_ASSERT(*executedCount == k_numTasks, "An incorrect amount of tasks were run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a batch of simple game logic tasks can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SimpleGameLogicTaskBatch)
            {
                constexpr u32 k_numTasks = 5;
                
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                
                std::vector<CSCore::TaskSchedulerNew::SimpleTask> tasks;
                for (u32 i = 0; i < k_numTasks; ++i)
                {
                    tasks.push_back([=]() noexcept
                    {
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedCount);
                    });
                }
                
                taskScheduler->ScheduleTasks(CSCore::TaskType::k_gameLogic, tasks, [=]() noexcept
                {
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_ASSERT(*executedCount == k_numTasks, "An incorrect amount of tasks were run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a batch of simple file tasks can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SimpleFileTaskBatch)
            {
                constexpr u32 k_numTasks = 5;
                
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                
                std::vector<CSCore::TaskSchedulerNew::SimpleTask> tasks;
                for (u32 i = 0; i < k_numTasks; ++i)
                {
                    tasks.push_back([=]() noexcept
                    {
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedCount);
                    });
                }
                
                taskScheduler->ScheduleTasks(CSCore::TaskType::k_file, tasks, [=]() noexcept
                {
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_ASSERT(*executedCount == k_numTasks, "An incorrect amount of tasks were run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a child small task can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ChildSmallTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
            
                taskScheduler->ScheduleTask(CSCore::TaskType::k_small,[=](const CSCore::TaskContext& in_parentTaskContext) noexcept
                {
                    CSIT_ASSERT(in_parentTaskContext.GetType() == CSCore::TaskType::k_small, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                    std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                    in_parentTaskContext.ProcessChildTask([=](const CSCore::TaskContext& in_childTaskContext) noexcept
                    {
                        CSIT_ASSERT(in_childTaskContext.GetType() == CSCore::TaskType::k_small, "Incorrect task type.");
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                        
                        ++(*executedCount);
                    });
                    
                    CSIT_ASSERT(*executedCount == 1, "The child task hasn't run.");
                    
                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a child large task can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ChildLargeTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                taskScheduler->ScheduleTask(CSCore::TaskType::k_large,[=](const CSCore::TaskContext& in_parentTaskContext) noexcept
                {
                    CSIT_ASSERT(in_parentTaskContext.GetType() == CSCore::TaskType::k_large, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                    std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                    in_parentTaskContext.ProcessChildTask([=](const CSCore::TaskContext& in_childTaskContext) noexcept
                    {
                        CSIT_ASSERT(in_childTaskContext.GetType() == CSCore::TaskType::k_large, "Incorrect task type.");
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedCount);
                    });

                    CSIT_ASSERT(*executedCount == 1, "The child task hasn't run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a child main thread task can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ChildMainThreadTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                taskScheduler->ScheduleTask(CSCore::TaskType::k_mainThread,[=](const CSCore::TaskContext& in_parentTaskContext) noexcept
                {
                    CSIT_ASSERT(in_parentTaskContext.GetType() == CSCore::TaskType::k_mainThread, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                    std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                    in_parentTaskContext.ProcessChildTask([=](const CSCore::TaskContext& in_childTaskContext) noexcept
                    {
                        CSIT_ASSERT(in_childTaskContext.GetType() == CSCore::TaskType::k_mainThread, "Incorrect task type.");
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedCount);
                    });

                    CSIT_ASSERT(*executedCount == 1, "The child task hasn't run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a child game logic task can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ChildGameLogicTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                taskScheduler->ScheduleTask(CSCore::TaskType::k_gameLogic,[=](const CSCore::TaskContext& in_parentTaskContext) noexcept
                {
                    CSIT_ASSERT(in_parentTaskContext.GetType() == CSCore::TaskType::k_gameLogic, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                    std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                    in_parentTaskContext.ProcessChildTask([=](const CSCore::TaskContext& in_childTaskContext) noexcept
                    {
                        CSIT_ASSERT(in_childTaskContext.GetType() == CSCore::TaskType::k_gameLogic, "Incorrect task type.");
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedCount);
                    });

                    CSIT_ASSERT(*executedCount == 1, "The child task hasn't run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a child file task can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ChildFileTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                taskScheduler->ScheduleTask(CSCore::TaskType::k_file,[=](const CSCore::TaskContext& in_parentTaskContext) noexcept
                {
                    CSIT_ASSERT(in_parentTaskContext.GetType() == CSCore::TaskType::k_file, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                    std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                    in_parentTaskContext.ProcessChildTask([=](const CSCore::TaskContext& in_childTaskContext) noexcept
                    {
                        CSIT_ASSERT(in_childTaskContext.GetType() == CSCore::TaskType::k_file, "Incorrect task type.");
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedCount);
                    });

                    CSIT_ASSERT(*executedCount == 1, "The child task hasn't run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a child simple small task can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ChildSimpleSmallTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                taskScheduler->ScheduleTask(CSCore::TaskType::k_small,[=](const CSCore::TaskContext& in_parentTaskContext) noexcept
                {
                    CSIT_ASSERT(in_parentTaskContext.GetType() == CSCore::TaskType::k_small, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                    std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                    in_parentTaskContext.ProcessChildTask([=]() noexcept
                    {
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedCount);
                    });

                    CSIT_ASSERT(*executedCount == 1, "The child task hasn't run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a child simple large task can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ChildSimpleLargeTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                taskScheduler->ScheduleTask(CSCore::TaskType::k_large,[=](const CSCore::TaskContext& in_parentTaskContext) noexcept
                {
                    CSIT_ASSERT(in_parentTaskContext.GetType() == CSCore::TaskType::k_large, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                    std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                    in_parentTaskContext.ProcessChildTask([=]() noexcept
                    {
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedCount);
                    });

                    CSIT_ASSERT(*executedCount == 1, "The child task hasn't run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a child simple main thread task can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ChildSimpleMainThreadTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                taskScheduler->ScheduleTask(CSCore::TaskType::k_mainThread,[=](const CSCore::TaskContext& in_parentTaskContext) noexcept
                {
                    CSIT_ASSERT(in_parentTaskContext.GetType() == CSCore::TaskType::k_mainThread, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                    std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                    in_parentTaskContext.ProcessChildTask([=]() noexcept
                    {
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedCount);
                    });

                    CSIT_ASSERT(*executedCount == 1, "The child task hasn't run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a child simple game logic task can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ChildSimpleGameLogicTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                taskScheduler->ScheduleTask(CSCore::TaskType::k_gameLogic,[=](const CSCore::TaskContext& in_parentTaskContext) noexcept
                {
                    CSIT_ASSERT(in_parentTaskContext.GetType() == CSCore::TaskType::k_gameLogic, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                    std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                    in_parentTaskContext.ProcessChildTask([=]() noexcept
                    {
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedCount);
                    });

                    CSIT_ASSERT(*executedCount == 1, "The child task hasn't run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a child simple file task can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ChildSimpleFileTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                taskScheduler->ScheduleTask(CSCore::TaskType::k_file,[=](const CSCore::TaskContext& in_parentTaskContext) noexcept
                {
                    CSIT_ASSERT(in_parentTaskContext.GetType() == CSCore::TaskType::k_file, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                    std::shared_ptr<std::atomic<u32>> executedCount(new std::atomic<u32>(0));
                    in_parentTaskContext.ProcessChildTask([=]() noexcept
                    {
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedCount);
                    });

                    CSIT_ASSERT(*executedCount == 1, "The child task hasn't run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that a batch of child small tasks can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ChildSmallTaskBatch)
            {
                //TODO: !? Implement
            }
            //------------------------------------------------------------------------------
            /// Confirms both that game logic tasks are executed during the same frame that
            /// they were scheduled and that main thread tasks scheduled within a game logic
            /// task are also executed with same frame.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(GameLogicTaskWithinFrame)
            {
                auto app = CSCore::Application::Get();
                auto taskScheduler = app->GetTaskSchedulerNew();
                
                taskScheduler->ScheduleTask(CSCore::TaskType::k_gameLogic, [=]() noexcept
                {
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    
                    taskScheduler->ScheduleTask(CSCore::TaskType::k_mainThread, [=]() noexcept
                    {
                        //TODO: !? Confirm that this is the same frame as when the game logic task was scheduled.
                        
                        CSIT_ASSERT(taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                        CSIT_PASS();
                    });
                });
            }
        }
    }
}
