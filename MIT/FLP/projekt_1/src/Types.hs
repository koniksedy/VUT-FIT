-- Types.hs
-- Module with a type of a Context Free Grammar
-- Project: SIMPLIFY-BKG (FIL)
-- Author: Michal Šedý <xsedym02@stud.fit.vutbr.cz>
-- Last change: 23.03.2020

module Types (Grammar(..), Rule(..), Symbol) where
import Data.List (intercalate)


type Symbol = Char

data Rule = Rule Symbol [Symbol]
    deriving (Eq)

-- Example of the output:
-- ----------------------
-- A->Ba
instance Show Rule
    where show (Rule x ys) = [x] ++ "->" ++ ys

data Grammar = Grammar
    {
        nonterms :: [Symbol],
        terms :: [Symbol],
        start :: Symbol,
        rules :: [Rule]
    }

-- Example of the output:
-- ----------------------
-- A,S
-- a,b
-- S->#
-- S->A
-- A->aA
-- A->b
instance Show Grammar
    where show (Grammar ns ts s rs) = unlines $ intercalate "," (map (:[]) ns) :
                                                intercalate "," (map (:[]) $ filter (/='#') ts) :
                                                [s] :
                                                map show rs
