module Main where

import qualified Data.ByteString as BS
import Data.Elf
import System.Environment
import Text.Show.Pretty

main :: IO ()
main = do
	args <- getArgs
	print $ args
	b <- BS.readFile $ args !! 0
	pPrint $ parseElf b
	BS.writeFile "text" $ elfSectionData $ head $ filter (\x -> elfSectionName x == ".text") $ elfSections $ parseElf b

