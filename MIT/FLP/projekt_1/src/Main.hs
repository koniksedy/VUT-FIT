-- Main.hs
-- The main module for FLP project 1.
-- Project: SIMPLIFY-BKG (FIL)
-- Author: Michal Šedý <xsedym02@stud.fit.vutbr.cz>
-- Last change: 23.03.2020

module Main where
import System.Environment (getArgs)

import ParseInput (createGrammar, parseOpts)
import Minimization (minimizeGrammar1, minimizeGrammar2)


lstripList :: [String] -> [String]
lstripList [] = []
lstripList (x:xs)
    | null x = lstripList xs
    | otherwise = x : xs

rstripList :: [String] -> [String]
rstripList xs = reverse $ lstripList $ reverse xs


main :: IO ()
main = do
    args <- getArgs
    let (readingInput, opt) = parseOpts args

    -- Print HELP if needed

    content <- readingInput
    let grammar = createGrammar $ rstripList $ lines content
    case opt of "-i" -> putStr $ show grammar
                "-1" -> putStr $ show $ minimizeGrammar1 grammar
                "-2" -> putStr $ show $ minimizeGrammar2 grammar
                "-h" -> mapM_ putStrLn ["USAGE", "./main <-i|-1|-2> [input]"]
                _ -> error "PROGRAM ERROR: case mismatch"
