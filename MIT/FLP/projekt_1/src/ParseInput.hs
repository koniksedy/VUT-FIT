-- ParseInput.hs
-- The module is parsing program parameters and input grammar.
-- Project: SIMPLIFY-BKG (FIL)
-- Author: Michal Šedý <xsedym02@stud.fit.vutbr.cz>
-- Last change: 23.03.2020

module ParseInput (createGrammar, parseOpts) where

import Types (Grammar(..), Rule(..), Symbol)
import Data.Char (isUpper, isLower)
import Data.List (isInfixOf)


-- Grammar creator
-- ----------------------------------------------------------------------------
createGrammar :: [String] -> Grammar
createGrammar (ns:ts:s:rs) = testGrammar (Grammar nonterminals' terminals' start' rules')
    where nonterminals' = parseNonterms ns
          terminals' = parseTerms ts
          start' = parseStart s
          rules' = map parseRules rs
createGrammar _ = error "Grammar is too short."


-- Grammar parsers
-------------------------------------------------------------------------------
parseNonterms :: String -> [Symbol]
parseNonterms [] = error "The grammar does not contain any nonterminals."
parseNonterms xs = splitToChars ',' xs

parseTerms :: String -> [Symbol]
parseTerms xs = '#' : splitToChars ',' xs

parseStart :: String -> Symbol
parseStart [] = error "No start symbol."
parseStart [x] = x
parseStart _ = error "The start nonterminal must be only one character."

parseRules :: String -> Rule
parseRules [] = error "PROGRAM ERROR: Empty rule."
parseRules (x:xs) = Rule x (skipArrow xs)


-- Grammar tester
-- ----------------------------------------------------------------------------
testGrammar :: Grammar -> Grammar
testGrammar g@(Grammar ns ts s _)
    | testNonterms && testTerms && testStart && testRules g = g
    | otherwise = error "Bad grammar format."
        where testNonterms = all isUpper ns
              testTerms = all (\x -> isLower x || x == '#') ts
              testStart = s `elem` ns

testRules :: Grammar -> Bool
testRules (Grammar _ _ _ []) = True
testRules (Grammar ns ts s (r@(Rule src dsts):rs))
    | r `elem` rs = False
    | otherwise = testR && testRules (Grammar ns ts s rs)
        where testR = src `elem` ns && all (\x -> x `elem` ts || x `elem` ns) dsts

-- Grammar helpers
-------------------------------------------------------------------------------
splitToChars :: Char -> String -> [Symbol]
splitToChars _ [] = []
splitToChars delim [x]
    | delim /= x = [x]
    | otherwise = error "No symbols only delimiter."
splitToChars delim (x:y:xs)
    | delim /= x && y == delim = x: splitToChars delim xs
    | otherwise = error ("Symbol" ++ show x ++ " is longer than one character.")

skipArrow :: String -> String
skipArrow xs
    | "->" `isInfixOf` xs = dropWhile (==' ') $ drop 2 $ dropWhile (==' ') xs
    | otherwise = error "Rule does not contain arrow."

-- Program parameters parser
-------------------------------------------------------------------------------
parseOpts :: [String] -> (IO String, String)
parseOpts args = case args of [] -> error "Too few program parameters were specified."
                              ["-i"] -> (getContents, "-i")
                              ["-1"] -> (getContents, "-1")
                              ["-2"] -> (getContents, "-2")
                              "-i" : [fileName] -> (readFile fileName, "-i")
                              "-1" : [fileName] -> (readFile fileName, "-1")
                              "-2" : [fileName] -> (readFile fileName, "-2")
                              _  | "-h" `elem` args || "--help" `elem` args -> (getContents, "-h")
                                 | otherwise -> error ("Unknown program parameters " ++ last args ++ ".")
