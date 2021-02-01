#include "defs.h"
#include "search.h"
#include <algorithm>
#include <iostream>

Search::Search(const Board &board, Limits limits, std::vector<ZKey> positionHistory, bool logUci) :
    _positionHistory(positionHistory),
    _limits(limits),
    _initialBoard(board),
    _logUci(logUci),
    _stop(false),
    _limitCheckCount(0),
    _bestScore(0) {

  if (_limits.infinite) { // Infinite search
    _searchDepth = INF;
    _timeAllocated = INF;
  } else if (_limits.depth != 0) { // Depth search
    _searchDepth = _limits.depth;
    _timeAllocated = INF;
  } else if (_limits.moveTime != 0) {
    _searchDepth = MAX_SEARCH_DEPTH;
    _timeAllocated = _limits.moveTime;
  } else if (_limits.time[_initialBoard.getActivePlayer()] != 0) { // Time search
    int ourTime = _limits.time[_initialBoard.getActivePlayer()];
    int opponentTime = _limits.time[_initialBoard.getInactivePlayer()];

    // Divide up the remaining time (If movestogo not specified we are in 
    // sudden death)
    if (_limits.movesToGo == 0) {
      // Allocate less time for this search if our opponent's time is greater
      // than our time by scaling movestogo by the ratio between our time
      // and our opponent's time (ratio max forced to 2.0, min forced to 1.0)
      double timeRatio = std::max((double) (ourTime / opponentTime), 1.0);

      int movesToGo = (int) (SUDDEN_DEATH_MOVESTOGO * std::min(2.0, timeRatio));
      _timeAllocated = ourTime / movesToGo;
    } else {
      // A small constant (3) is added to _limits.movesToGo when dividing to
      // ensure we don't go over time when movesToGo is small
      _timeAllocated = ourTime / (_limits.movesToGo + 3);
    }

    // Use all of the increment to think
    _timeAllocated += _limits.increment[_initialBoard.getActivePlayer()];

    // Depth is infinity in a timed search (ends when time runs out)
    _searchDepth = MAX_SEARCH_DEPTH;
  } else { // No limits specified, use default depth
    _searchDepth = DEFAULT_SEARCH_DEPTH;
    _timeAllocated = INF;
  }
}

void Search::stop() {
  _stop = true;
}

void Search::iterDeep() {
  _start = std::chrono::steady_clock::now();

  for (int currDepth = 1; currDepth <= _searchDepth; currDepth++) {
    _rootMax(_initialBoard, currDepth);
  }

  if (_logUci) std::cout << "bestmove " << getBestMove().getNotation() << std::endl;
}

Move Search::getBestMove() {
  return _bestMove;
}

void Search::_rootMax(const Board &board, int depth) {
  MoveGen movegen(board);
  MoveList legalMoves = movegen.getLegalMoves();
  _nodes = 0;

  // If no legal moves are available, just return, setting bestmove to a null move
  if (legalMoves.empty()) {
    _bestMove = Move();
    _bestScore = -INF;
    return;
  }

    _bestMove = legalMoves.at(0);
    _bestScore = 0;
}
