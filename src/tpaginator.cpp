/* Copyright (c) 2011-2012, AOYAMA Kazuharu
 * All rights reserved.
 *
 * This software may be used and distributed according to the terms of
 * the New BSD License, which is incorporated herein by reference.
 *
 * Author: Darko Goleš
 * Author: Carlos Mafla <gigo6000@hotmail.com>
 * Author: Vo Xuan Tien <tien.xuan.vo@gmail.com>
 * Modified by AOYAMA Kazuharu
 */

#include <TPaginator>
#include <QtCore>

/*!
  \class TPaginator
  \brief The TPaginator class provides simple solution to show a paging bar.
*/

/*!
  Constructs a TPaginator object using the parameters.
  \a itemsCount specifies the number of items.
  \a limit specifies the maximum number of items to be shown per page.
  \a midRange specifies the number of page numbers to be shown on a paging
  bar, and should be an odd number.
*/
TPaginator::TPaginator(int itemsCount, int limit, int midRange)
    : currentPage_(1)
{
    itemsCount_ = qMax(itemsCount, 0);
    limit_ = qMax(limit, 1);

    // Change even number to larger odd number.
    // midRange must be odd number.
    midRange = qMax(midRange, 1);
    midRange_ = midRange + (((midRange % 2) == 0) ? 1 : 0);

    calculateNumPages();
    calculateOffset();
    calculateRange();
}

/*!
  Calculates number of pages.
  Internal use only.
*/
void TPaginator::calculateNumPages()
{
    //If limit is larger than or equal to total items count
    //display all in one page
    if (limit_ >= itemsCount_) {
        numPages_ = 1;
    } else {
        //Calculate rest numbers from dividing operation so we can add one
        //more page for this items
        int restItemsNum = itemsCount_ % limit_;
        //if rest items > 0 then add one more page else just divide items
        //by limit
        numPages_ = (restItemsNum > 0) ? (itemsCount_ / limit_) + 1 : (itemsCount_ / limit_);
    }

    // If currentPage invalid after numPages changes
    if (currentPage_ > numPages_) {
        // Restore currentPage to default value
        currentPage_ = 1;
    }
}

/*!
  Calculates the offset that means the number of items before the first
  item of the current page.
  Internal use only.
*/
void TPaginator::calculateOffset()
{
    offset_ = (currentPage_ - 1) * limit_;
}

/*!
  Calculates the range of the paging bar.
  Internal use only.
*/
void TPaginator::calculateRange()
{
    int startRange = currentPage_ - qFloor(midRange_ / 2);
    int endRange = currentPage_ + qFloor(midRange_ / 2);

    // If invalid start range
    startRange = qMax(startRange, 1);

    // If invalid end range
    endRange = qMin(endRange, numPages_);

    range_.clear();
    for (int i = startRange; i <= endRange; i++) {
        range_ << i;
    }
}

/*!
  Sets the number of items to \a count and recalculates other parameters.
*/
void TPaginator::setItemsCount(int count)
{
    itemsCount_ = qMax(count, 0);

    // ItemsCount changes cause NumPages and Range change
    calculateNumPages();
    calculateRange();
}

/*!
  Sets the maximum number of items to be shown per page to \a limit,
  and recalculates other parameters.
*/
void TPaginator::setLimit(int limit)
{
    limit_ = qMax(limit, 1);

    // Limit changes cause NumPages, Offset and Range change
    calculateNumPages();
    calculateOffset();
    calculateRange();
}

/*!
  Sets the number of page numbers to \a range and recalculates other
  parameters.
*/
void TPaginator::setMidRange(int range)
{
    // Change even number to larger odd number
    range = qMax(range, 1);
    midRange_ = range + (((range % 2) == 0) ? 1 : 0);

    // MidRange changes cause Range changes
    calculateRange();
}

/*!
  Sets the current page to \a page and recalculates other parameters.
*/
void TPaginator::setCurrentPage(int page)
{
    currentPage_ = isValidPage(page) ? page : 1;

    // CurrentPage changes cause Offset and Range change
    calculateOffset();
    calculateRange();
}

/*!
  \fn int TPaginator::itemsCount() const
  Returns the number of items.
*/

/*!
  \fn int TPaginator::numPages() const
  Returns the total number of pages.
*/

/*!
  \fn int TPaginator::limit() const
  Returns the maximum number of items to be shown per page.
*/

/*!
  \fn int TPaginator::offset() const
  Returns the number of items before the first item of the current
  page.
*/

/*!
  \fn int TPaginator::midRange() const
  Returns the number of page numbers to be shown on a paging bar.
*/

/*!
  \fn const QList<int> &TPaginator::range() const
  Returns a list of page numbers to be shown on a paging bar.
*/

/*!
  \fn int TPaginator::currentPage() const
  Returns the current page.
*/

/*!
  \fn int TPaginator::firstPage() const
  Returns the first page.
*/

/*!
  \fn int TPaginator::previousPage() const
  Returns the page before the current page.
*/

/*!
  \fn int TPaginator::nextPage() const
  Returns the page after current page.
*/

/*!
  \fn int TPaginator::lastPage() const
  Returns the last page.
*/

/*!
  \fn bool TPaginator::haveToPaginate() const
  Returns true if the number of items is greater than the maximum
  number of items to be shown per page; otherwise returns false.
*/

/*!
  \fn bool TPaginator::isFirstPage() const
  Returns true if the current is the first page; otherwise
  returns false.
*/

/*!
  \fn bool TPaginator::hasPreviousPage() const
  Returns true if there is at least one page before the current page;
  otherwise returns false.
*/

/*!
  \fn bool TPaginator::hasNextPage() const
  Returns true if there is at least one page after the current page;
  otherwise returns false.
*/

/*!
  \fn bool TPaginator::isLastPage() const
  Returns true if the current is the last page; otherwise returns false.
*/

/*!
  \fn bool TPaginator::isValidPage(int page) const
  Returns true if \a page is a valid page; otherwise returns false.
*/
