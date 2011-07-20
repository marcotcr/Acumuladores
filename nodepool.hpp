// Copyright (c) 2009-2011 by Tiago Alves Macambira <tmacam@burocrata.org>
//
// All rights reserved.
//
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#ifndef NODEPOOL_HPP_
#define NODEPOOL_HPP_

#include <map>
#include <memory>

#include "mempools.hpp"
#include "nodetypes.hpp"
#include "sigslot.h"

#include "counter.cc"

/**A simple node executive that needs to be guided to run.
 *
 * A NodePool has only the necessary machinery to "store" nodes
 * (hence the pool) but lacks the needed code to actually carry out an
 * execution (i.e., node loops).
 *
 * This is left for other classes to perform , such as @see NodeConductor.
 * Communication between this class the the one actually conducting the
 * loop-execution is done by means of signals and slots (events).
 *
 *
 */
class NodePool : public AbstractNodeExecutive, public sigslot::has_slots<> {
 public:
	typedef std::map<node_id_t, AbstractNode*> node_map_t;
	typedef std::map<node_id_t, mailbox_t*> nodes_mailboxes_t;

	NodePool();

	virtual ~NodePool();

	/**@name NodePoolIfaceMethods
	 *
	 * AbstractNodeExecutive && AbstractNodeEnvironment methods
	 */
	//@{
	virtual void voteForNextLoop();
	virtual void sendMessage(node_id_t to, const MsgBuffer& msg);

	/** Output  the result of some node's computation to the env.
	 *
	 * This method just delegates the output to listeners of
	 * performOutputResult signal.
	 */
	virtual void outputResult(const std::string& result);

	virtual void addNode(AbstractNode* node);
	//@}

	/**@name NodePoolSignals
	 * NodePool Signals.
	 *
	 * These signals are used by NodePool to notify others of a
	 * change in its state or to delegate the responsability of
	 * performing something.
	 */
	//@{
	/**@Signals that the current iteration ended for all nodes in this pool.
	 *
	 * As single parameter it provides the number of votes
	 * for the next iteration.
	 */
	sigslot::signal1<int> iterationEnded;
	/**A message delivery was requested by a node.
	 *
	 * First parameter is the destination node's id.
	 *
	 * Second parameter is the message contents.
	 *
	 */
	sigslot::signal2<node_id_t, const MsgBuffer&> messageDelivered;
	/**Requests that node's output be properly performed.
	 *
	 * @see outputResult
	 *
	 */
	sigslot::signal1<const std::string&> performOutputResult;
	//@}
	

	/**@name NodePoolSlots
	 *
	 * NodePool's "Slots". Usually, you will connect signals on those
	 * methods.
	 */
	//@{
	//!Notify nodes of loop's start and calls their OnBegin.
	void loopBegin ();
	//!Perform a loop iteration.
	void loopIteration(int loop_count);
	//!Notify nodes of loop's termination and calls their OnEnd.
	void loopEnd();
	//!Perform a "low-level" mesage delivery to a local node's inbox.
	void directSendMessage(node_id_t to, const MsgBuffer& msg);
	//@}
  
  void IncrementCounter(string name, int id);

 private:
  void swapAndClearAllMailboxes();

  int votes_for_next_loop_;
  node_map_t nodes_;

  ///@name Nodes' mailboxes for current and next supersteps
  ///{
  nodes_mailboxes_t cur_sstep_msgs_;
  nodes_mailboxes_t next_sstep_msgs_;
  ///}

  ///@name Pools were messages' contents are allocated.
  ///{
  std::auto_ptr<ExpandableMemPool> prev_sstep_pool_;
  std::auto_ptr<ExpandableMemPool> next_sstep_pool_;
  ///}
  

  CounterManager counter_manager_;

  DISALLOW_COPY_AND_ASSIGN(NodePool);
};


#endif  // NODEPOOL_HPP_
// vim:syn=cpp.doxygen:autoindent:smartindent:fileencoding=utf-8:fo+=tcroq:
