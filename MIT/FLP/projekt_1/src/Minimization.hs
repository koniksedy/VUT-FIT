-- Minimization.hs
-- The module is deleting unnecessary symbols.
-- Project: SIMPLIFY-BKG (FIL)
-- Author: Michal Šedý <xsedym02@stud.fit.vutbr.cz>
-- Last change: 23.03.2020

module Minimization where

import Data.Set (Set, fromList, toList, empty, insert, intersection, union, member)
import Types (Grammar(..), Rule(..), Symbol)

-- Find nonterminals that generates terminal string
generNonterms :: Set Symbol -> Grammar -> Set Symbol
generNonterms acc g
    | acc == nextAcc = acc
    | otherwise = generNonterms nextAcc g
        where nextAcc = fromList [src | (Rule src dsts) <- rules g, all (\x -> x `member` acc || x `elem` terms g) dsts]

-- Find symbols that are reachable from the start nonterminal.
reachableSymbols :: Set Symbol -> Grammar -> Set Symbol
reachableSymbols acc g
    | acc == empty = error ("PROGRAM ERROR: starting symbol must be reachable" ++ show acc)
    | acc == nextAcc = acc
    | otherwise = reachableSymbols nextAcc g
        where nextAcc = union acc $ fromList [dst | (Rule src dsts) <- rules g, src `member` acc, dst <- dsts]

-- Remove nongenerating terminals and unreachable symbols.
minimizeGrammar2 :: Grammar -> Grammar
minimizeGrammar2 g = let nonterms' = generNonterms empty g
                         rules' = [Rule src dsts | (Rule src dsts) <- rules g, src `member` nonterms', all (\x -> x `member` nonterms' || x `elem` terms g) dsts]
                         reachable = reachableSymbols (fromList [start g]) (Grammar (toList $ insert (start g) nonterms') (terms g) (start g) rules')
                         nonterms'' = toList $ insert (start g) $ intersection nonterms' reachable
                         terms'' = toList $ intersection (fromList $ terms g) reachable
                         rules'' = [Rule src dsts | (Rule src dsts) <- rules g, src `elem` nonterms'', all (`member` reachable) dsts]
                      in Grammar nonterms'' terms'' (start g) rules''

-- Remove nongenerating terminals.
minimizeGrammar1 :: Grammar -> Grammar
minimizeGrammar1 g = let nonterms' = generNonterms empty g
                         rules' = [Rule src dsts | (Rule src dsts) <- rules g, src `member` nonterms', all (\x -> x `member` nonterms' || x `elem` terms g) dsts]
                     in Grammar (toList $ insert (start g) nonterms') (terms g) (start g) rules'
