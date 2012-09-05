main = interact respondPalidromes

shortLinesOnly :: String -> String
shortLinesOnly input = 
  let allLines = lines input
      shortLines = filter (\line -> length line < 10) allLines
      result = unlines shortLines
  in result

respondPalidromes = unlines . map (\xs -> 
                                    if isPalidrome xs then xs ++ "palindrome" else "not a palidrome") . lines
  where isPalidrome xs = xs == reverse xs
