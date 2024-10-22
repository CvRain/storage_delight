export default async function Page() {
  const att:number = 0
    return (
      <div className="flex w-full flex-col md:col-span-4">
        <h2 className='main'>
          Latest Invoices {att}
          <Page2/>
        </h2>
      </div>
    );
}
export async function Page2() {
  const att:number = 44444444
    return (
      <div className="flex w-full flex-col md:col-span-4">
        <h2 className='main'>
          Latest Invoices {att}
        </h2>
      </div>
    );
}
  