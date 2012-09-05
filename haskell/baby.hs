import Data.List

doubleMe x = x + x
doubleUs x y = doubleMe x + doubleMe y
doubleSmallNumber x = if x > 100
                      then x
                      else x*2

first :: (a, b, c) -> a
first (x, _, _) = x

second :: (a, b, c) -> b
second (_, y, _) = y

third :: (a, b, c) -> c
third (_, _, z) = z

tail' :: [a] -> a
tail' [] = error "Cant"
tail' (_:[x]) = x

length' :: (Num b) => [a] -> b
length' [] = 0
length' (_:xs) = 1 + length' xs

bmiTell :: (RealFloat a) => a -> a -> String
bmiTell weight height
  | bmi <= skinny = "You're underweight"
  | bmi <= normal = "you're normal"
  | bmi <= fat = "you're fat"
  | otherwise = "You're a whale"
  where bmi = weight / height ^ 2
        (skinny, normal, fat) = (18.5, 25.0, 30.0)
        
initials :: String -> String -> String
initials firstname lastname = [f] ++ ", " ++ [l] ++ ", "
    where (f:_) = firstname
          (l:_) = lastname
          
calcBmis :: (RealFloat a) => [(a, a)] -> [a]
calcBmis xs = [ bmi w h | (w, h) <- xs ]
    where bmi weight height = weight / height ^ 2 
          
maximum' :: (Ord a) => [a] -> a
maximum' [] = error "maximum of empty list"
maximum' [x] = x
maximum' (x:xs) = max x (maximum' xs)

zip' :: [a] -> [b] -> [(a, b)]
zip' _ [] = []
zip' [] _ = []
zip (x:xs) (y:ys) = (x, y):zip' xs ys

quicksort :: (Ord a) => [a] -> [a]
quicksort [] = []
quicksort (x:xs) = 
  let smallerSerted = quicksort [a | a <- xs, a <= x]
      biggerSorted = quicksort [a | a <- xs, a > x]
  in smallerSerted ++ [x] ++ biggerSorted
     
qsort' :: (Ord a) => [a] -> [a]
qsort' [] = []
qsort' (x:xs) = qsort' [ a | a <- xs, a <= x ]
               ++ [x] ++
               qsort' [ a | a <- xs, a > x ]
               
chain :: (Integral a) => a -> [a]
chain 1 = [1]
chain n
  | even n = n:chain (n `div` 2)
  | odd n = n:chain (n*3 + 1)
            
numLongChains :: Int
numLongChains = length (filter isLong (map chain [1..100]))
    where isLong xs = length xs > 15

search :: (Eq a) => [a] -> [a] -> Bool
search needle haystack = 
  let nlen = length needle
  in foldl (\acc x -> if take nlen x == needle then True else acc) False (tails haystack)