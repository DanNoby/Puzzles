from copy import deepcopy
import os 
import time 

class Table:
  table = None
  heuristic = None
  winState = [1, 2, 3, 4, 5, 6, 7, 8, 0]

  def __init__(self, arr):
    self.table = arr
    self.CalcHeuristic()

  def __repr__(self):
    temp = deepcopy(self.table)
    temp[temp.index(0)] = ' '
    return f"{temp[0]} {temp[1]} {temp[2]}\n{temp[3]} {temp[4]} {temp[5]}\n{temp[6]} {temp[7]} {temp[8]}\n"

  # the heuristic here is simply the number of tiles not in their desired spot (as dictated by winState) 
  def CalcHeuristic(self):
    h = 0
    for i in range(len(self.winState)-1):
      if(self.table[i]!=self.winState[i]):
        h = h+1
    self.heuristic = h   

  def moves(self):
    possibleMoves = []
    space = self.table.index(0)

        # Left move
    if space % 3 > 0:
      r = Table(deepcopy(self.table)) 
      r.table[space], r.table[space - 1] = r.table[space - 1], r.table[space]
      r.CalcHeuristic()
      possibleMoves.append(r)

        # Right move
    if space % 3 < 2:
      t = Table(deepcopy(self.table))
      t.table[space], t.table[space + 1] = t.table[space + 1], t.table[space]
      t.CalcHeuristic()
      possibleMoves.append(t)

        # Up move
    if space - 3 >= 0:
      v = Table(deepcopy(self.table))
      v.table[space], v.table[space - 3] = v.table[space - 3], v.table[space]
      v.CalcHeuristic()
      possibleMoves.append(v)

        # Down move
    if space + 3 < 9:
      a = Table(deepcopy(self.table))
      a.table[space], a.table[space + 3] = a.table[space + 3], a.table[space]
      a.CalcHeuristic()
      possibleMoves.append(a)

    return possibleMoves

  def play(self):
    print(self)
    visited = set()
    backtrackPoss = []

    while self.table != self.winState:
      visited.add(tuple(self.table))

        # Find possible moves that have not been visited
      M = [move for move in self.moves() if tuple(move.table) not in visited]

        # Backtracking if no new moves available
      if not M:
        if backtrackPoss:
          print("Backtracking...")
          self.table = backtrackPoss.pop().table
          continue
        else:
          print("No solution found")
          return

            # Find moves with the smallest heuristic
      H = [move.heuristic for move in M]
      minHeuristic = min(H)

      # Update backtrackPoss with all other moves
      bestMove = M[H.index(minHeuristic)]
      for move in M:
        if move==bestMove:
          continue
        backtrackPoss.append(move)

      self.table = bestMove.table

      time.sleep(1)
      os.system('clear')
      print(self)

    print("Solved!")

table = eval(input())
T = Table(table)
T.play()

