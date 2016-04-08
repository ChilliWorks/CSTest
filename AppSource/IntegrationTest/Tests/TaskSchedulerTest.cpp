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

#include <array>

namespace CSTest
{
    namespace IntegrationTest
    {
        namespace
        {
            constexpr std::array<CSCore::TaskType, 4> k_backgroundTaskTypes =
            {
                CSCore::TaskType::k_small,
                CSCore::TaskType::k_large,
                CSCore::TaskType::k_gameLogic,
                CSCore::TaskType::k_file
            };
        }
        
        //------------------------------------------------------------------------------
        /// A series of integration tests for the Task Scheduler.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        CSIT_TESTCASE(TaskScheduler)
        {
            //------------------------------------------------------------------------------
            /// Tests that a task can be scheduled on the main thread.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ScheduleMainThreadTask)
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
            /// Tests that each type of background task can be scheduled on a background
            /// thread.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ScheduleBackgroundTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                std::shared_ptr<std::atomic<u32>> taskTypesPassed(new std::atomic<u32>(0));
                
                for (const auto & taskType : k_backgroundTaskTypes)
                {
                    taskScheduler->ScheduleTask(taskType, [=](const CSCore::TaskContext& in_taskContext) noexcept
                    {
                        CSIT_ASSERT(in_taskContext.GetType() == taskType, "Incorrect task type.");
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        if (++(*taskTypesPassed) >= k_backgroundTaskTypes.size())
                        {
                            CSIT_PASS();
                        }
                    });
                }
            }
            //------------------------------------------------------------------------------
            /// Confirms that a batch of main thread tasks can be scheduled.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ScheduleMainThreadTaskBatch)
            {
                constexpr u32 k_numTasks = 5;
                
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                std::shared_ptr<std::atomic<u32>> executedTaskCount(new std::atomic<u32>(0));
                
                std::vector<CSCore::TaskSchedulerNew::Task> tasks;
                for (u32 i = 0; i < k_numTasks; ++i)
                {
                    tasks.push_back([=](const CSCore::TaskContext& in_taskContext) noexcept
                    {
                        CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_mainThread, "Incorrect task type.");
                        CSIT_ASSERT(taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        ++(*executedTaskCount);
                    });
                }
                
                taskScheduler->ScheduleTasks(CSCore::TaskType::k_mainThread, tasks, [=](const CSCore::TaskContext& in_taskContext) noexcept
                {
                    CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_mainThread, "Incorrect task type.");
                    CSIT_ASSERT(taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_ASSERT(*executedTaskCount == k_numTasks, "An incorrect amount of tasks were run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that each type of background task can be scheduled as a batch
            /// on a background thread.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ScheduleBackgroundTaskBatch)
            {
                constexpr u32 k_numTasksPerTaskType = 5;
                
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                std::shared_ptr<std::atomic<u32>> taskTypesPassed(new std::atomic<u32>(0));
                
                for (const auto & taskType : k_backgroundTaskTypes)
                {
                    std::shared_ptr<std::atomic<u32>> executedTaskCount(new std::atomic<u32>(0));
                    
                    std::vector<CSCore::TaskSchedulerNew::Task> tasks;
                    for (u32 i = 0; i < k_numTasksPerTaskType; ++i)
                    {
                        tasks.push_back([=](const CSCore::TaskContext& in_taskContext) noexcept
                        {
                            CSIT_ASSERT(in_taskContext.GetType() == taskType, "Incorrect task type.");
                            CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                            
                            ++(*executedTaskCount);
                        });
                    }
                    
                    taskScheduler->ScheduleTasks(taskType, tasks, [=](const CSCore::TaskContext& in_taskContext) noexcept
                    {
                        CSIT_ASSERT(in_taskContext.GetType() == taskType, "Incorrect task type.");
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                        CSIT_ASSERT(*executedTaskCount == k_numTasksPerTaskType, "An incorrect amount of tasks were run.");
                        
                        if (++(*taskTypesPassed) >= k_backgroundTaskTypes.size())
                        {
                            CSIT_PASS();
                        }
                    });
                }
            }
            //------------------------------------------------------------------------------
            /// Confirms that main thread tasks can be scheduled as a child task.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ScheduleMainThreadChildTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                taskScheduler->ScheduleTask(CSCore::TaskType::k_mainThread, [=](const CSCore::TaskContext& in_parentTaskContext) noexcept
                {
                    CSIT_ASSERT(in_parentTaskContext.GetType() == CSCore::TaskType::k_mainThread, "Incorrect task type.");
                    CSIT_ASSERT(taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                    std::atomic<bool> taskExecuted(false);
                    in_parentTaskContext.ProcessChildTask([=, &taskExecuted](const CSCore::TaskContext& in_childTaskContext) noexcept
                    {
                        CSIT_ASSERT(in_childTaskContext.GetType() == CSCore::TaskType::k_mainThread, "Incorrect task type.");
                        CSIT_ASSERT(taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        taskExecuted = true;
                    });

                    CSIT_ASSERT(taskExecuted, "The child task hasn't run.");

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that all background task types can be scheduled as a child task.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ScheduleBackgroundChildTask)
            {
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
            
                std::shared_ptr<std::atomic<u32>> taskTypesPassed(new std::atomic<u32>(0));
                
                for (const auto & taskType : k_backgroundTaskTypes)
                {
                    taskScheduler->ScheduleTask(taskType, [=](const CSCore::TaskContext& in_parentTaskContext) noexcept
                    {
                        CSIT_ASSERT(in_parentTaskContext.GetType() == taskType, "Incorrect task type.");
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        std::atomic<bool> taskExecuted(false);
                        in_parentTaskContext.ProcessChildTask([=, &taskExecuted](const CSCore::TaskContext& in_childTaskContext) noexcept
                        {
                            CSIT_ASSERT(in_childTaskContext.GetType() == taskType, "Incorrect task type.");
                            CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                            
                            taskExecuted = true;
                        });
                        
                        CSIT_ASSERT(taskExecuted, "The child task hasn't run.");
                        
                        if (++(*taskTypesPassed) >= k_backgroundTaskTypes.size())
                        {
                            CSIT_PASS();
                        }
                    });
                }
            }
            //------------------------------------------------------------------------------
            /// Confirms that main thread tasks can be scheduled as a batch of child tasks.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ScheduleMainThreadChildTaskBatch)
            {
                constexpr u32 k_numTasks = 5;
                
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                taskScheduler->ScheduleTask(CSCore::TaskType::k_mainThread, [=](const CSCore::TaskContext& in_parentTaskContext) noexcept
                {
                    CSIT_ASSERT(in_parentTaskContext.GetType() == CSCore::TaskType::k_mainThread, "Incorrect task type.");
                    CSIT_ASSERT(taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                    std::atomic<u32> executedTaskCount(0);
                    std::vector<CSCore::TaskSchedulerNew::Task> tasks;
                    for (u32 i = 0; i < k_numTasks; ++i)
                    {
                        tasks.push_back([=, &executedTaskCount](const CSCore::TaskContext& in_taskContext) noexcept
                        {
                            CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_mainThread, "Incorrect task type.");
                            CSIT_ASSERT(taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                            
                            ++executedTaskCount;
                        });
                    }
                    
                    in_parentTaskContext.ProcessChildTasks(tasks);
                    
                    CSIT_ASSERT(executedTaskCount == k_numTasks, "An incorrect amount of tasks were run.");
                    
                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that all background task types can be scheduled as a batch of child
            /// tasks.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ScheduleBackgroundChildTaskBatch)
            {
                constexpr u32 k_numTasksPerTaskType = 5;
                
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                std::shared_ptr<std::atomic<u32>> taskTypesPassed(new std::atomic<u32>(0));
                
                for (const auto & taskType : k_backgroundTaskTypes)
                {
                    taskScheduler->ScheduleTask(taskType, [=](const CSCore::TaskContext& in_parentTaskContext) noexcept
                    {
                        CSIT_ASSERT(in_parentTaskContext.GetType() == taskType, "Incorrect task type.");
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                        std::atomic<u32> executedTaskCount(0);
                        std::vector<CSCore::TaskSchedulerNew::Task> tasks;
                        for (u32 i = 0; i < k_numTasksPerTaskType; ++i)
                        {
                            tasks.push_back([=, &executedTaskCount](const CSCore::TaskContext& in_taskContext) noexcept
                            {
                                CSIT_ASSERT(in_taskContext.GetType() == taskType, "Incorrect task type.");
                                CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                                ++executedTaskCount;
                            });
                        }

                        in_parentTaskContext.ProcessChildTasks(tasks);

                        CSIT_ASSERT(executedTaskCount == k_numTasksPerTaskType, "An incorrect amount of tasks were run.");

                        if (++(*taskTypesPassed) >= k_backgroundTaskTypes.size())
                        {
                            CSIT_PASS();
                        }
                    });
                }
            }
            //------------------------------------------------------------------------------
            /// Confirms that a task batches can be triple nested.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ScheduleNestedTaskBatch)
            {
                constexpr u32 k_numTasksPerLevel = 5;
                constexpr u32 k_numLevels = 3;
                
                auto taskScheduler = CSCore::Application::Get()->GetTaskSchedulerNew();
                
                std::shared_ptr<std::atomic<u32>> executedLevel3TaskCount(new std::atomic<u32>(0));
                
                std::vector<CSCore::TaskSchedulerNew::Task> level1Tasks;
                for (u32 level1TaskIndex = 0; level1TaskIndex < k_numTasksPerLevel; ++level1TaskIndex)
                {
                    level1Tasks.push_back([=](const CSCore::TaskContext& in_level1TaskContext) noexcept
                    {
                        CSIT_ASSERT(in_level1TaskContext.GetType() == CSCore::TaskType::k_small, "Incorrect task type.");
                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                        
                        std::vector<CSCore::TaskSchedulerNew::Task> level2Tasks;
                        for (u32 level2TaskIndex = 0; level2TaskIndex < k_numTasksPerLevel; ++level2TaskIndex)
                        {
                            level2Tasks.push_back([=](const CSCore::TaskContext& in_level2TaskContext) noexcept
                            {
                                CSIT_ASSERT(in_level2TaskContext.GetType() == CSCore::TaskType::k_small, "Incorrect task type.");
                                CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                                
                                std::vector<CSCore::TaskSchedulerNew::Task> level3Tasks;
                                for (u32 level3TaskIndex = 0; level3TaskIndex < k_numTasksPerLevel; ++level3TaskIndex)
                                {
                                    level3Tasks.push_back([=](const CSCore::TaskContext&) noexcept
                                    {
                                        CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");

                                        ++(*executedLevel3TaskCount);
                                    });
                                }
                                
                                in_level2TaskContext.ProcessChildTasks(level3Tasks);
                            });
                        }
                        
                        in_level1TaskContext.ProcessChildTasks(level2Tasks);
                    });
                }
                
                taskScheduler->ScheduleTasks(CSCore::TaskType::k_small, level1Tasks, [=](const CSCore::TaskContext& in_taskContext) noexcept
                {
                    CSIT_ASSERT(in_taskContext.GetType() == CSCore::TaskType::k_small, "Incorrect task type.");
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    CSIT_ASSERT(*executedLevel3TaskCount == u32(std::pow(k_numTasksPerLevel, k_numLevels)), "An incorrect amount of tasks were run.");

                    CSIT_PASS();
                });
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
                
                auto scheduledFrameIndex = app->GetFrameIndex();
                
                taskScheduler->ScheduleTask(CSCore::TaskType::k_gameLogic, [=](const CSCore::TaskContext&) noexcept
                {
                    CSIT_ASSERT(!taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                    
                    taskScheduler->ScheduleTask(CSCore::TaskType::k_mainThread, [=](const CSCore::TaskContext&) noexcept
                    {
                        CSIT_ASSERT(taskScheduler->IsMainThread(), "Task run on incorrect thread.");
                        CSIT_ASSERT(scheduledFrameIndex == app->GetFrameIndex(), "Task was not run within the same frame as it was scheduled.");
                        
                        CSIT_PASS();
                    });
                });
            }
        }
    }
}
