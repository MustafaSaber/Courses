package recfun

object Main {
  def main(args: Array[String]) {
    println("Pascal's Triangle")
    for (row <- 0 to 10) {
      for (col <- 0 to row)
        print(pascal(col, row) + " ")
      println()
    }
  }

  /**
   * Exercise 1
   */
    def pascal(c: Int, r: Int): Int = {
      if(c == 0 || r == c) 1
      else pascal(c-1 , r-1) + pascal(c, r-1)
    }
  
  /**
   * Exercise 2
   */
    def balance(chars: List[Char]): Boolean = {

      def Balance(chars: List[Char] , count: Int): Boolean = {
        if(chars.isEmpty) count == 0
        else
          {
            if(chars.head == '(')
              Balance(chars.tail , count+1)
            else
              if(chars.head == ')')
                count > 0 && Balance(chars.tail , count-1)
              else
                Balance(chars.tail , count)
          }
      }
      Balance(chars , 0)
    }
  
  /**
   * Exercise 3
   */
    def countChange(money: Int, coins: List[Int]): Int = {
      if(money == 0) 1
      else if(money > 0 && !coins.isEmpty)
          countChange(money - coins.head , coins) + countChange(money, coins.tail)
      else
        0
    }
  }
